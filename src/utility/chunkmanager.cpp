#include "chunkmanager.hpp"

ChunkManager::ChunkManager()
{
    ChunkLoadList = new std::vector<Chunk*>();
    ChunkMeshList = new std::vector<Chunk*>();
    ChunkVisibleList = new std::vector<Chunk*>();
    ChunkRenderList = new std::vector<Chunk*>();
    ChunkEmptyList = new std::vector<Chunk*>();
    ChunkUnloadList = new std::vector<Chunk*>();
    ChunkStore = new ChunkStorage();

    _debug = new DebugObject();
    _generator = new ChunkGenerator(new TerrainGenerator());
    _mesher = new ChunkMesher();
}

ChunkManager::~ChunkManager()
{
    while(WorldVariables::NUM_GEN_THREADS+WorldVariables::NUM_MESH_THREADS > 0)
    {
        std::cout << "\tWaiting for Threads..." << std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
    }
    std::cout << "\tDeleting Lists" << std::endl;
    delete ChunkLoadList;
    delete ChunkMeshList;
    delete ChunkVisibleList;
    delete ChunkRenderList;
    delete ChunkEmptyList;
    delete ChunkUnloadList;
    std::cout << "\tDeleting ChunkStore" << std::endl;
    delete ChunkStore;
    std::cout << "\tDeleting Debugger" << std::endl;
    delete _debug;
    std::cout << "\tDeleting Generator" << std::endl;
    delete _generator;
    std::cout << "\tDeleting Mesher" << std::endl;
    delete _mesher;
}

void ChunkManager::Update(Camera* camera, GLuint distance)
{
    if(!WorldVariables::FREEZE_RENDERLIST)
    {
        RequestChunks(camera);
        UpdateLoadList();
        UpdateMeshList();
        UpdateVisibleList(camera);
        UpdateEmptyList();
        UpdateUnloadList();
    }
}

void ChunkManager::UpdateEmptyList()
{
    uint maxRms = WorldVariables::CHUNK_UPDATES_PER_FRAME*2, rms = 0;
    auto start = std::chrono::high_resolution_clock::now();
    for(uint i = 0; i < ChunkEmptyList->size() && rms<maxRms; i++)
    {
        Chunk* c = ChunkEmptyList->at(i);
        if(!ChunkInViewDistance(c))
        {
            ChunkEmptyList->at(i) = ChunkEmptyList->back();
            ChunkEmptyList->pop_back();
            ChunkUnloadList->push_back(c);
        }
    }
    WorldVariables::EMPTYLIST_TIME = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now()-start).count();
}

void ChunkManager::UpdateUnloadList()
{
    uint maxRms = WorldVariables::CHUNK_UPDATES_PER_FRAME*2, rms = 0;
    auto start = std::chrono::high_resolution_clock::now();
    for(uint i = 0; i < ChunkUnloadList->size() && rms<maxRms; i++)
    {
        Chunk* c = ChunkUnloadList->at(i);
        if(!c->inUse && ((c->isMeshed || !c->isMeshing) || (c->isGenerated || !c->isGenerating)))
        {
            ChunkUnloadList->at(i) = ChunkUnloadList->back();
            ChunkUnloadList->pop_back();
            ChunkStore->DeleteChunk(c->chunkX, c->chunkY, c->chunkZ);
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

    for(uint i = 0; i < ChunkVisibleList->size(); i++)
    {
        Chunk* a = ChunkVisibleList->at(i);
        if(!ChunkInViewDistance(a))
        {
            ChunkVisibleList->at(i) = ChunkVisibleList->back();
            ChunkVisibleList->pop_back();
            ChunkUnloadList->push_back(a);
        }
        else if(a->GetMesh()->EmptyMesh())
        {
            ChunkVisibleList->at(i) = ChunkVisibleList->back();
            ChunkVisibleList->pop_back();
            ChunkEmptyList->push_back(a);
        }
        else if(!a->isMeshed)
        {
            ChunkVisibleList->at(i) = ChunkVisibleList->back();
            ChunkVisibleList->pop_back();
            ChunkMeshList->push_back(a);
        }
        else if(a->ShouldRender(0, &c->Front, &c->Position))
        {
            ChunkRenderList->push_back(a);
        }
    }
    WorldVariables::VISIBLELIST_TIME = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now()-start).count();
}

void ChunkManager::UpdateMeshList()
{
    auto start = std::chrono::high_resolution_clock::now();
    uint maxLoads = WorldVariables::CHUNK_UPDATES_PER_FRAME;
    uint curLoads = 0;
    for(uint i = 0; i < ChunkMeshList->size()&&curLoads < maxLoads; i++)
    {
        Chunk* c = ChunkMeshList->at(i);
        if(!ChunkInViewDistance(c))
        {
            ChunkMeshList->at(i) = ChunkMeshList->back();
            ChunkMeshList->pop_back();
            ChunkUnloadList->push_back(c);
        }
        else if(c->isMeshed)
        {
            ChunkMeshList->at(i) = ChunkMeshList->back();
            ChunkMeshList->pop_back();
            ChunkVisibleList->push_back(c);
        }
        else if (!c->isMeshing)
        {
            std::string hTop = ChunkStore->ChunkHash(c->chunkX, c->chunkY+1, c->chunkZ);
            std::string hBottom = ChunkStore->ChunkHash(c->chunkX, c->chunkY-1, c->chunkZ);
            std::string hRight = ChunkStore->ChunkHash(c->chunkX+1, c->chunkY, c->chunkZ);
            std::string hLeft = ChunkStore->ChunkHash(c->chunkX-1, c->chunkY, c->chunkZ);
            std::string hFront = ChunkStore->ChunkHash(c->chunkX, c->chunkY, c->chunkZ+1);
            std::string hBack = ChunkStore->ChunkHash(c->chunkX, c->chunkY, c->chunkZ-1);
            if( ChunkStore->ChunkExists(hTop) && ChunkStore->ChunkExists(hBottom) && ChunkStore->ChunkExists(hRight) &&
                ChunkStore->ChunkExists(hLeft) && ChunkStore->ChunkExists(hFront) && ChunkStore->ChunkExists(hBack))
            {
                Chunk* top =    ChunkStore->GetChunk(hTop);
                Chunk* bottom = ChunkStore->GetChunk(hBottom);
                Chunk* right =  ChunkStore->GetChunk(hRight);
                Chunk* left =   ChunkStore->GetChunk(hLeft);
                Chunk* front =  ChunkStore->GetChunk(hFront);
                Chunk* back =   ChunkStore->GetChunk(hBack);

                if(top->isGenerated && bottom->isGenerated && right->isGenerated && left->isGenerated && front->isGenerated && back->isGenerated)
                {
                    top->inUse = bottom->inUse = right->inUse = left->inUse = front->inUse = back->inUse = true;
                    _mesher->GenerateChunkMesh(c, top, bottom, right, left, front, back);
                    c->isMeshing = true;
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
    uint s = ChunkLoadList->size();
    uint maxLoads = WorldVariables::CHUNK_UPDATES_PER_FRAME;
    uint curLoads = 0;
    for(uint i = 0; i < ChunkLoadList->size()&&curLoads < maxLoads; i++)
    {
        Chunk* c = ChunkLoadList->at(i);
        if(c->isGenerated)
        {
            ChunkLoadList->at(i) = ChunkLoadList->back();
            ChunkLoadList->pop_back();
            if(ChunkInViewDistance(c))
            {
                if(c->ShouldMesh())
                    ChunkMeshList->push_back(c);
                else
                    ChunkEmptyList->push_back(c);
            }
            else
            {
                ChunkUnloadList->push_back(c);
            }
        }
        else if(!c->isGenerating)
        {
            _generator->GenerateChunk(c);
            c->isGenerating = true;
            curLoads++;
        }
    }
    WorldVariables::LOADLIST_TIME += std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now()-start).count();
    WorldVariables::LOADLIST_TIME /= 2;
}

void ChunkManager::RequestChunks(Camera* c)
{
    auto start = std::chrono::high_resolution_clock::now();
    int V_D = WorldVariables::VIEW_DISTANCE;
    glm::vec3 newChunk = WorldVariables::CUR_CHUNK;
    if(newChunk!=_curChunk)
    {
        _curChunk = newChunk;
        for(int x = -V_D; x <= V_D; x++)
            for(int z = -V_D; z <= V_D; z++)
                for(int y = -V_D/2; y <= V_D/2; y++)
                {
                    if(!ChunkStore->ChunkExists(_curChunk.x+x, _curChunk.y+y, _curChunk.z+z))
                    {
                        ChunkLoadList->push_back(ChunkStore->InsertChunk(_curChunk.x+x, _curChunk.y                                             +y, _curChunk.z+z));
                    }
                }  
    }
    WorldVariables::LOADCHUNKS_TIME = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now()-start).count();
}

void ChunkManager::ClearRenderList()
{
    if(!WorldVariables::FREEZE_RENDERLIST)
        ChunkRenderList->clear();
}

void ChunkManager::Render(glm::mat4* view, glm::mat4* proj, glm::mat4* mvp)
{
    auto start = std::chrono::high_resolution_clock::now();
    Mesh::SetupShader();
    for(Chunk*c: *ChunkRenderList)
    {
        if(c->isMeshed)
        {
            c->Render(view, proj, mvp);
        }
    }

    if(WorldVariables::SHOW_RENDER_BOUNDS)
    {
        for(Chunk*c: *ChunkRenderList)
        {
            _debug->DrawDebugChunk(c->chunkX, c->chunkY, c->chunkZ, *proj, *view, glm::vec3(1,1,1));
        }
    }

    if(WorldVariables::SHOW_LOAD_BOUNDS)
    {
        for(Chunk*c: *ChunkLoadList)
        {
            _debug->DrawDebugChunk(c->chunkX, c->chunkY, c->chunkZ, *proj, *view, glm::vec3(0,1,0));
        }
    }

    if(WorldVariables::SHOW_UNLOAD_BOUNDS)
    {
        for(Chunk*c: *ChunkUnloadList)
        {
            _debug->DrawDebugChunk(c->chunkX, c->chunkY, c->chunkZ, *proj, *view, glm::vec3(1,0,0));
        }
    }

    if(WorldVariables::SHOW_VISIBLE_BOUNDS)
    {
        for(Chunk*c: *ChunkVisibleList)
        {
            _debug->DrawDebugChunk(c->chunkX, c->chunkY, c->chunkZ, *proj, *view, glm::vec3(0,0,1));
        }
    }

    if(WorldVariables::SHOW_MESH_BOUNDS)
    {
        for(Chunk*c: *ChunkMeshList)
        {
            _debug->DrawDebugChunk(c->chunkX, c->chunkY, c->chunkZ, *proj, *view, glm::vec3(1,0,1));
        }
    }

    if(WorldVariables::SHOW_EMPTY_BOUNDS)
    {
        for(Chunk*c: *ChunkEmptyList)
        {
            _debug->DrawDebugChunk(c->chunkX, c->chunkY, c->chunkZ, *proj, *view, glm::vec3(0.5,0.5,0.5));
        }
    }

    WorldVariables::RENDERLIST_TIME = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now()-start).count();
}

bool ChunkManager::BlockExistsInChunk(int x, int y, int z, int cx, int cy, int cz)
{
    int CHUNK_SIZE = WorldVariables::CHUNK_SIZE;
    int wxb = (cx * CHUNK_SIZE);
    int wyb = (cy * CHUNK_SIZE);
    int wzb = (cz * CHUNK_SIZE);
    int wy = wyb+y;

    if(wy<100)
        return true;

    FastNoiseSIMD* p_gen = FastNoiseSIMD::NewFastNoiseSIMD();
    p_gen->SetFrequency(0.002);
    p_gen->SetPerturbAmp(1.9);
    p_gen->SetFractalOctaves(1.5);
    p_gen->SetPerturbFractalLacunarity(1.2);
    p_gen->SetFractalGain(0.4);
    //float* p_set = p_gen->GetSimplexFractalSet(wxb+x, 0, wzb+z, 1, 1, 1);
    float* p_set = p_gen->GetSimplexSet(wxb+x, 0, wzb+z, 1, 1, 1);

    float val = p_set[0];
    val = 100+(val*(156));
    val = abs(val);

    FastNoiseSIMD::FreeNoiseSet(p_set);
    delete p_gen;
    return ((wy<val)?true:false);
}

bool ChunkManager::ChunkInViewDistance(Chunk *c)
{
    int V_D = WorldVariables::VIEW_DISTANCE;
    glm::vec3 cc = WorldVariables::CUR_CHUNK;
    int cx = c->chunkX;
    int cy = c->chunkY;
    int cz = c->chunkZ;
    return abs(cx-cc.x) <= V_D && abs(cy-cc.y) <= V_D/2 && abs(cz-cc.z) <= V_D;
}
