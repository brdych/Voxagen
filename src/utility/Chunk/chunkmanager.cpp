#include "chunkmanager.hpp"
#include "utility/Voxagen/voxagenengine.hpp"
#include "utility/3P/ThreadPool-master/ThreadPool.h"
#include "utility/Chunk/chunkhelper.hpp"
#include "utility/3P/ThreadPool-master/savevtask.hpp"
#include "utility/3P/ThreadPool-master/meshvtask.hpp"

ChunkManager::ChunkManager()
{
    ChunkLoadList = std::vector<Chunk*>();
    ChunkMeshList = std::vector<Chunk*>();
    ChunkVisibleList = std::vector<Chunk*>();
    ChunkRenderList = std::vector<Chunk*>();
    ChunkEmptyList = std::vector<Chunk*>();
    ChunkUnloadList = std::vector<Chunk*>();

    _debug = DebugObject();
    _mesher = ChunkMesher();
}

void ChunkManager::Reset()
{
    delete VoxagenEngine::POOL;
    VoxagenEngine::POOL = new ThreadPool(VoxagenEngine::THREAD_COUNT);
    ChunkLoadList.clear();
    ChunkLoadList.shrink_to_fit();
    ChunkMeshList.clear();
    ChunkMeshList.shrink_to_fit();
    ChunkVisibleList.clear();
    ChunkVisibleList.shrink_to_fit();
    ChunkRenderList.clear();
    ChunkRenderList.shrink_to_fit();
    ChunkEmptyList.clear();
    ChunkEmptyList.shrink_to_fit();
    ChunkUnloadList.clear();
    ChunkUnloadList.shrink_to_fit();

    ChunkStore.ChangeSize(static_cast<uint>(WorldVariables::VIEW_DISTANCE));
    _numThreads = 0;
    _refresh = true;
}

ChunkManager::~ChunkManager()
{

}

void ChunkManager::SaveChunks()
{
    for(Chunk* c: ChunkVisibleList)
    {
        if(c->playerData)
        {
            VTask* t = new SaveVTask(c);
            t->execute();
            delete t;
        }
    }
}

void ChunkManager::Update()
{
    if(!WorldVariables::FREEZE_RENDERLIST)
    {
        if(WorldVariables::EDIT_MODE)
        {
            _debug.DrawDebugCube(WorldVariables::EDIT_POS.x, WorldVariables::EDIT_POS.y, WorldVariables::EDIT_POS.z,VoxagenEngine::PROJ_MAT,VoxagenEngine::VIEW_MAT);
        }
        //std::cout << "Request" << std::endl;
        RequestChunks();
        //std::cout << "LoadList" << std::endl;
        UpdateLoadList();
        //std::cout << "MeshList" << std::endl;
        UpdateMeshList();
        //std::cout << "VisibleList" << std::endl;
        UpdateVisibleList(&VoxagenEngine::CAMERA);
        //std::cout << "EmptyList" << std::endl;
        UpdateEmptyList();
        //std::cout << "Unload" << std::endl;
        UpdateUnloadList();
        //std::cout << "Finish" << std::endl;
    }
}

void ChunkManager::UpdateEmptyList()
{
    auto start = std::chrono::high_resolution_clock::now();
    for(uint i = 0; i < ChunkEmptyList.size(); i++)
    {
        auto c = ChunkEmptyList.at(i);
         c->DeInit();
        if(!ChunkInViewDistance(c))
        {
            ChunkEmptyList.at(i) = ChunkEmptyList.back();
            ChunkEmptyList.pop_back();
            ChunkUnloadList.push_back(c);
        }
        else if(c->rebuild)
        {
            c->rebuild = false;
            ChunkEmptyList.at(i) = ChunkEmptyList.back();
            ChunkEmptyList.pop_back();
            ChunkMeshList.push_back(c);
        }
    }
    ChunkEmptyList.shrink_to_fit();
    WorldVariables::EMPTYLIST_TIME = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now()-start).count();
}

void ChunkManager::UpdateUnloadList()
{
    uint maxRms = WorldVariables::CHUNK_UPDATES_PER_FRAME, rms = 0;
    auto start = std::chrono::high_resolution_clock::now();
    for(uint i = 0; i < ChunkUnloadList.size() && rms<maxRms; i++)
    {
        auto c = ChunkUnloadList.at(i);
        if((c->UseStatus() == 0) && ((c->isMeshed || !c->isMeshing) || (c->isGenerated || !c->isGenerating)))
        {
            ChunkUnloadList.at(i) = ChunkUnloadList.back();
            ChunkUnloadList.pop_back();
            ChunkStore.DeleteChunkV2(c);
            rms++;
        }
        if(VoxagenEngine::DEBUG_LIST)
        {
            c->PrintDebug();
        }
    }
    ChunkUnloadList.shrink_to_fit();
    WorldVariables::UNLOADLIST_TIME = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now()-start).count();
}

void ChunkManager::UpdateVisibleList(Camera* c)
{
    auto start = std::chrono::high_resolution_clock::now();
    for(uint i = 0; i < ChunkVisibleList.size(); i++)
    {
        auto a = ChunkVisibleList.at(i);
        if(a->forceRemesh)
        {
            ChunkVisibleList.at(i) = ChunkVisibleList.back();
            ChunkVisibleList.pop_back();
            ChunkRenderList.push_back(a);
            ChunkMeshList.insert(ChunkMeshList.begin(), a);
        }
        else
        {
            if(!ChunkInViewDistance(a))
            {
                if(a->playerData)
                {
                    VoxagenEngine::POOL->enqueue(new SaveVTask(a));
                }
                else {
                    ChunkVisibleList.at(i) = ChunkVisibleList.back();
                    ChunkVisibleList.pop_back();
                    ChunkUnloadList.push_back(a);
                }
            }
            else if(a->GetMesh(0)->EmptyMesh() && a->GetMesh(1)->EmptyMesh())
            {
                ChunkVisibleList.at(i) = ChunkVisibleList.back();
                ChunkVisibleList.pop_back();
                ChunkEmptyList.push_back(a);
            }
            else if(!a->isMeshed)
            {
                ChunkVisibleList.at(i) = ChunkVisibleList.back();
                ChunkVisibleList.pop_back();
                ChunkMeshList.push_back(a);
            }
            else if(a->ShouldRender(0, &c->Front, &c->Position))
            {
                ChunkRenderList.push_back(a);
            }
        }
    }
    ChunkVisibleList.shrink_to_fit();
    WorldVariables::VISIBLELIST_TIME = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now()-start).count();
}

void ChunkManager::UpdateMeshList()
{
    auto start = std::chrono::high_resolution_clock::now();
    uint maxLoads = WorldVariables::CHUNK_UPDATES_PER_FRAME/2;
    uint curLoads = 0;
    for(uint i = 0; i < ChunkMeshList.size()&&curLoads < maxLoads; i++)
    {
        auto c = ChunkMeshList.at(i);
        if(!ChunkInViewDistance(c))
        {
            ChunkMeshList.at(i) = ChunkMeshList.back();
            ChunkMeshList.pop_back();
            ChunkUnloadList.push_back(c);
        }
        else if(c->isMeshed)
        {
            ChunkMeshList.at(i) = ChunkMeshList.back();
            ChunkMeshList.pop_back();
            ChunkVisibleList.push_back(c);
            --_numThreads;
        }
        else if (!c->isMeshing /*&& _numThreads < VoxagenEngine::THREAD_COUNT*/)
        {
            int x = c->chunkX;
            int y = c->chunkY;
            int z = c->chunkZ;

            if( ChunkStore.ChunkExistsV2(x, y+1, z) &&
                ChunkStore.ChunkExistsV2(x, y-1, z) &&
                ChunkStore.ChunkExistsV2(x+1, y, z) &&
                ChunkStore.ChunkExistsV2(x-1, y, z) &&
                ChunkStore.ChunkExistsV2(x, y, z+1) &&
                ChunkStore.ChunkExistsV2(x, y, z-1))
            {
                auto top =    ChunkStore.GetChunkV2(x, y+1, z);
                auto bottom = ChunkStore.GetChunkV2(x, y-1, z);
                auto right =  ChunkStore.GetChunkV2(x+1, y, z);
                auto left =   ChunkStore.GetChunkV2(x-1, y, z);
                auto front =  ChunkStore.GetChunkV2(x, y, z+1);
                auto back =   ChunkStore.GetChunkV2(x, y, z-1);
                if(top->isGenerated && bottom->isGenerated && right->isGenerated && left->isGenerated && front->isGenerated && back->isGenerated)
                {
                    top->RegisterUse();
                    bottom->RegisterUse();
                    right->RegisterUse();
                    left->RegisterUse();
                    front->RegisterUse();
                    back->RegisterUse();
                    c->isMeshing = true;
                    if(c->forceRemesh)
                    {
                        VTask* t = new MeshVTask(c, top, bottom, right, left, front, back);
                        t->execute();
                        delete t;
                        c->forceRemesh = false;
                        c->rebuild = false;
                        ChunkMeshList.at(i) = ChunkMeshList.back();
                        ChunkMeshList.pop_back();
                        ChunkVisibleList.push_back(c);
                    }
                    else
                    {
                        _mesher.GenerateChunkMesh(c, top, bottom, right, left, front, back);
                        ++curLoads;
                    }
                    //++_numThreads;
                }
            }
        }
    }
    ChunkMeshList.shrink_to_fit();
    WorldVariables::MESHLIST_TIME += std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now()-start).count();
    WorldVariables::MESHLIST_TIME /= 2;
}

void ChunkManager::UpdateLoadList()
{
    auto start = std::chrono::high_resolution_clock::now();
    uint maxLoads = WorldVariables::CHUNK_UPDATES_PER_FRAME;
    uint curLoads = 0;
    for(uint i = 0; i < ChunkLoadList.size()&&curLoads < maxLoads; i++)
    {
        auto c = ChunkLoadList.at(i);
        if(c->isGenerated)
        {
            _numThreads--;
            WorldVariables::CHUNKSTORE_STORED_BYTES += c->byteSize;
            ChunkLoadList.at(i) = ChunkLoadList.back();
            ChunkLoadList.pop_back();
            if(ChunkInViewDistance(c))
            {
                if(c->ShouldMesh())
                    ChunkMeshList.push_back(c);
                else
                    ChunkEmptyList.push_back(c);
            }
            else
            {
                ChunkUnloadList.push_back(c);
            }
        }
        else if(!c->isGenerating /*&& _numThreads < VoxagenEngine::THREAD_COUNT*/)
        {
            _generator.GenerateChunk(c);
            c->isGenerating = true;
            curLoads++;
            //++_numThreads;
        }
    }
    ChunkLoadList.shrink_to_fit();
    WorldVariables::LOADLIST_TIME += std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now()-start).count();
    WorldVariables::LOADLIST_TIME /= 2;
}

void ChunkManager::RequestChunks()
{
    auto start = std::chrono::high_resolution_clock::now();
    int V_D = WorldVariables::VIEW_DISTANCE;
    glm::ivec3 newChunk = WorldVariables::CUR_CHUNK;
    if(newChunk!=_curChunk||_refresh)
    {
        _curChunk = newChunk;
        _refresh = false;
        for(int x = -V_D; x <= V_D; ++x)
            for(int z = -V_D; z <= V_D; ++z)
                for(int y = -V_D; y <= V_D; ++y)
                {
                    if(!ChunkStore.ChunkExistsV2(_curChunk.x+x, _curChunk.y+y, _curChunk.z+z))
                    {
                        ChunkLoadList.push_back(ChunkStore.InsertChunkV2(_curChunk.x+x, _curChunk.y+y, _curChunk.z+z));
                    }
                }  
    }
    WorldVariables::LOADCHUNKS_TIME = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now()-start).count();
}

void ChunkManager::ClearRenderList()
{
    if(!WorldVariables::FREEZE_RENDERLIST)
    {
        ChunkRenderList.clear();
        ChunkRenderList.shrink_to_fit();
    }
}

void ChunkManager::Render(glm::mat4* view, glm::mat4* proj, glm::mat4* mvp)
{
    auto start = std::chrono::high_resolution_clock::now();
    Mesh::SetupShader();
    for(auto c: ChunkRenderList)
    {
        if(c->isMeshed || c->forceRemesh)
        {
            c->Render(view, proj, mvp);
        }
    }

    Mesh::SetupWaterShader();
    for(Chunk*c: ChunkRenderList)
    {
        if(c->isMeshed|| c->forceRemesh)
        {
            c->RenderWater(view, proj, mvp);
        }
    }

    if(WorldVariables::SHOW_RENDER_BOUNDS)
    {
        for(Chunk*c: ChunkRenderList)
        {
            _debug.DrawDebugChunk(c->chunkX, c->chunkY, c->chunkZ, *proj, *view, glm::vec3(1,1,1));
        }
    }

    if(WorldVariables::SHOW_LOAD_BOUNDS)
    {
        for(Chunk*c: ChunkLoadList)
        {
            _debug.DrawDebugChunk(c->chunkX, c->chunkY, c->chunkZ, *proj, *view, glm::vec3(0,1,0));
        }
    }

    if(WorldVariables::SHOW_UNLOAD_BOUNDS)
    {
        for(Chunk*c: ChunkUnloadList)
        {
            _debug.DrawDebugChunk(c->chunkX, c->chunkY, c->chunkZ, *proj, *view, glm::vec3(1,0,0));
        }
    }

    if(WorldVariables::SHOW_VISIBLE_BOUNDS)
    {
        for(Chunk*c: ChunkVisibleList)
        {
            _debug.DrawDebugChunk(c->chunkX, c->chunkY, c->chunkZ, *proj, *view, glm::vec3(0,0,1));
        }
    }

    if(WorldVariables::SHOW_MESH_BOUNDS)
    {
        for(Chunk*c: ChunkMeshList)
        {
            _debug.DrawDebugChunk(c->chunkX, c->chunkY, c->chunkZ, *proj, *view, glm::vec3(1,0,1));
        }
    }

    if(WorldVariables::SHOW_EMPTY_BOUNDS)
    {
        for(Chunk*c: ChunkEmptyList)
        {
            _debug.DrawDebugChunk(c->chunkX, c->chunkY, c->chunkZ, *proj, *view, glm::vec3(0.5,0.5,0.5));
        }
    }

    WorldVariables::RENDERLIST_TIME = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now()-start).count();
}

bool ChunkManager::ChunkInViewDistance(Chunk *c)
{
    uint V_D = static_cast<uint>(WorldVariables::VIEW_DISTANCE);
    glm::ivec3 cc = WorldVariables::CUR_CHUNK;
    int cx = c->chunkX;
    int cy = c->chunkY;
    int cz = c->chunkZ;
    return ChunkHelper::DistanceBetween(cx,cc.x) <= V_D && ChunkHelper::DistanceBetween(cy,cc.y) <= V_D && ChunkHelper::DistanceBetween(cz,cc.z) <= V_D;
}
