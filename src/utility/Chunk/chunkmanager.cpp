#include "chunkmanager.hpp"
#include "utility/Voxagen/voxagenengine.hpp"

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
    ChunkLoadList.clear();
    ChunkMeshList.clear();
    ChunkVisibleList.clear();
    ChunkRenderList.clear();
    ChunkEmptyList.clear();
    ChunkUnloadList.clear();
    ChunkStore.ChangeSize(WorldVariables::VIEW_DISTANCE);
    _refresh = true;
}

ChunkManager::~ChunkManager()
{

}

void ChunkManager::SaveChunks()
{
    if(WorldVariables::USE_GORP)
    {
        for(Chunk* c : ChunkVisibleList)
        {
            VoxagenEngine::GORP_IO->SaveChunk(c);
        }
    }
    else
    {
        for(Chunk* c : ChunkVisibleList)
        {
            VoxagenEngine::FILE_IO->SaveChunk(c);
        }
    }
    WorldVariables::SAVE_CHUNK_STORE = false;
    WorldVariables::FREEZE_RENDERLIST = false;
}

void ChunkManager::Update()
{
    if(!WorldVariables::FREEZE_RENDERLIST && !WorldVariables::SAVE_CHUNK_STORE)
    {
        if(WorldVariables::EDIT_MODE)
        {
            _debug.DrawDebugCube(WorldVariables::LOOKING_AT.x,WorldVariables::LOOKING_AT.y,WorldVariables::LOOKING_AT.z,VoxagenRenderer::PROJ_MAT,VoxagenRenderer::VIEW_MAT);
        }
        //std::cout << "Request" << std::endl;
        RequestChunks();
        //std::cout << "LoadList" << std::endl;
        UpdateLoadList();
        //std::cout << "MeshList" << std::endl;
        UpdateMeshList();
        //std::cout << "VisibleList" << std::endl;
        UpdateVisibleList(&VoxagenRenderer::CAMERA);
        //std::cout << "EmptyList" << std::endl;
        UpdateEmptyList();
        //std::cout << "Unload" << std::endl;
        UpdateUnloadList();
        //std::cout << "Finish" << std::endl;
    }
    else if(WorldVariables::SAVE_CHUNK_STORE)
    {
        SaveChunks();
    }
}

void ChunkManager::UpdateEmptyList()
{
    uint maxRms = WorldVariables::CHUNK_UPDATES_PER_FRAME*2, rms = 0;
    auto start = std::chrono::high_resolution_clock::now();
    for(uint i = 0; i < ChunkEmptyList.size() && rms<maxRms; i++)
    {
        Chunk* c = ChunkEmptyList.at(i);
        if(!ChunkInViewDistance(c))
        {
            ChunkEmptyList.at(i) = ChunkEmptyList.back();
            ChunkEmptyList.pop_back();
            ChunkUnloadList.push_back(c);
        }
        else if(c->rebuild)
        {
            ChunkEmptyList.at(i) = ChunkEmptyList.back();
            ChunkEmptyList.pop_back();
            ChunkMeshList.push_back(c);
        }
    }
    WorldVariables::EMPTYLIST_TIME = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now()-start).count();
}

void ChunkManager::UpdateUnloadList()
{
    uint maxRms = WorldVariables::CHUNK_UPDATES_PER_FRAME*2, rms = 0;
    auto start = std::chrono::high_resolution_clock::now();
    for(uint i = 0; i < ChunkUnloadList.size() && rms<maxRms; i++)
    {
        Chunk* c = ChunkUnloadList.at(i);
        if(!c->inUse && ((c->isMeshed || !c->isMeshing) || (c->isGenerated || !c->isGenerating)))
        {
            ChunkUnloadList.at(i) = ChunkUnloadList.back();
            ChunkUnloadList.pop_back();
            ChunkStore.DeleteChunkV2(c);
            rms++;
        }
    }
    WorldVariables::UNLOADLIST_TIME = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now()-start).count();
}

void ChunkManager::UpdateVisibleList(Camera* c)
{
    auto start = std::chrono::high_resolution_clock::now();
    int CHUNK_SIZE = WorldVariables::CHUNK_SIZE;
    int distance = 20;
    int px = floor(c->Position.x)/CHUNK_SIZE;
    int py = floor(c->Position.y)/CHUNK_SIZE;
    int pz = floor(c->Position.z)/CHUNK_SIZE;

    for(uint i = 0; i < ChunkVisibleList.size(); i++)
    {
        Chunk* a = ChunkVisibleList.at(i);
        if(!ChunkInViewDistance(a))
        {
            ChunkVisibleList.at(i) = ChunkVisibleList.back();
            ChunkVisibleList.pop_back();
            ChunkUnloadList.push_back(a);
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
    WorldVariables::VISIBLELIST_TIME = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now()-start).count();
}

void ChunkManager::UpdateMeshList()
{
    auto start = std::chrono::high_resolution_clock::now();
    uint maxLoads = WorldVariables::CHUNK_UPDATES_PER_FRAME/2;
    uint curLoads = 0;
    for(uint i = 0; i < ChunkMeshList.size()&&curLoads < maxLoads; i++)
    {
        Chunk* c = ChunkMeshList.at(i);
        if(!ChunkInViewDistance(c))
        {
            ChunkMeshList.at(i) = ChunkMeshList.back();
            ChunkMeshList.pop_back();
            ChunkUnloadList.push_back(c);
        }
        else if(c->isMeshed)
        {
            CurrentThreads--;
            ChunkMeshList.at(i) = ChunkMeshList.back();
            ChunkMeshList.pop_back();
            ChunkVisibleList.push_back(c);
        }
        else if (!c->isMeshing)
        {
            if( ChunkStore.ChunkExistsV2(c->chunkX, c->chunkY+1, c->chunkZ) &&
                ChunkStore.ChunkExistsV2(c->chunkX, c->chunkY-1, c->chunkZ) &&
                ChunkStore.ChunkExistsV2(c->chunkX+1, c->chunkY, c->chunkZ) &&
                ChunkStore.ChunkExistsV2(c->chunkX-1, c->chunkY, c->chunkZ) &&
                ChunkStore.ChunkExistsV2(c->chunkX, c->chunkY, c->chunkZ+1) &&
                ChunkStore.ChunkExistsV2(c->chunkX, c->chunkY, c->chunkZ-1))
            {
                Chunk* top =    ChunkStore.GetChunkV2(c->chunkX, c->chunkY+1, c->chunkZ);
                Chunk* bottom = ChunkStore.GetChunkV2(c->chunkX, c->chunkY-1, c->chunkZ);
                Chunk* right =  ChunkStore.GetChunkV2(c->chunkX+1, c->chunkY, c->chunkZ);
                Chunk* left =   ChunkStore.GetChunkV2(c->chunkX-1, c->chunkY, c->chunkZ);
                Chunk* front =  ChunkStore.GetChunkV2(c->chunkX, c->chunkY, c->chunkZ+1);
                Chunk* back =   ChunkStore.GetChunkV2(c->chunkX, c->chunkY, c->chunkZ-1);
                if(top->isGenerated && bottom->isGenerated && right->isGenerated && left->isGenerated && front->isGenerated && back->isGenerated && CurrentThreads < _threadcount)
                {
                    CurrentThreads++;
                    top->inUse = bottom->inUse = right->inUse = left->inUse = front->inUse = back->inUse = true;
                    c->isMeshing = true;
                    _mesher.GenerateChunkMesh(c, top, bottom, right, left, front, back);
                    curLoads++;
                }
            }
        }
    }
    WorldVariables::MESHLIST_TIME += std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now()-start).count();
    WorldVariables::MESHLIST_TIME /= 2;
}

void ChunkManager::UpdateLoadList()
{
    auto start = std::chrono::high_resolution_clock::now();
    uint s = ChunkLoadList.size();
    uint maxLoads = WorldVariables::CHUNK_UPDATES_PER_FRAME;
    uint curLoads = 0;
    for(uint i = 0; i < ChunkLoadList.size()&&curLoads < maxLoads; i++)
    {
        Chunk* c = ChunkLoadList.at(i);
        if(c->isGenerated)
        {
            CurrentThreads--;
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
        else if(!c->isGenerating && CurrentThreads < _threadcount)
        {
            CurrentThreads++;
            _generator.GenerateChunk(c);
            c->isGenerating = true;
            curLoads++;
        }
    }
    WorldVariables::LOADLIST_TIME += std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now()-start).count();
    WorldVariables::LOADLIST_TIME /= 2;
}

void ChunkManager::RequestChunks()
{
    auto start = std::chrono::high_resolution_clock::now();
    int V_D = WorldVariables::VIEW_DISTANCE;
    glm::vec3 newChunk = WorldVariables::CUR_CHUNK;
    if(newChunk!=_curChunk||_refresh)
    {
        _curChunk = newChunk;
        _refresh = false;
        for(int x = -V_D; x <= V_D; x++)
            for(int z = -V_D; z <= V_D; z++)
                for(int y = -V_D; y <= V_D; y++)
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
        ChunkRenderList.clear();
}

void ChunkManager::Render(glm::mat4* view, glm::mat4* proj, glm::mat4* mvp)
{
    auto start = std::chrono::high_resolution_clock::now();
    Mesh::SetupShader();
    for(Chunk*c: ChunkRenderList)
    {
        if(c->isMeshed)
        {
            c->Render(view, proj, mvp);
        }
    }

    Mesh::SetupWaterShader();
    for(Chunk*c: ChunkRenderList)
    {
        if(c->isMeshed)
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
    int V_D = WorldVariables::VIEW_DISTANCE;
    glm::vec3 cc = WorldVariables::CUR_CHUNK;
    int cx = c->chunkX;
    int cy = c->chunkY;
    int cz = c->chunkZ;
    return abs(cx-cc.x) <= V_D && abs(cy-cc.y) <= V_D && abs(cz-cc.z) <= V_D;
}
