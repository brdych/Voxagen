#include "chunkmanager.hpp"
#include "math.h"

ChunkManager* ChunkManager::_instance;
bool ChunkManager::SHUTDOWN = false;

void UpdateLoadList()
{
    ChunkManager* cm = ChunkManager::ChunkManagerInstance();
    while(!ChunkManager::SHUTDOWN)
    {
        if(cm->ChunkLoadList->size() > 0)
        {
            cm->ChunkLoadMutex.lock();
            //std::cout <<"Mutex Locked by Loader: Size:"<< cm->ChunkLoadList->size() << std::endl;
            Chunk* c = nullptr;
            if(cm->ChunkLoadList->size()>0)
            {
                c = cm->ChunkLoadList->back();
                cm->ChunkLoadList->pop_back();
            }
            //std::cout <<"Mutex Unlock by Loader: Size:"<< cm->ChunkLoadList->size() << std::endl;
            cm->ChunkLoadMutex.unlock();
            if(c != nullptr)
            {
                c->BuildVoxelData();
                if(c->ShouldMesh())
                {
                    cm->ChunkSetupMutex.lock();
                    cm->ChunkSetupList->push_back(c);
                    cm->ChunkSetupMutex.unlock();
                }
                else
                {
                    delete c;
                }
            }
        }
    }
}

void UpdateSetupList()
{
    ChunkManager* cm = ChunkManager::ChunkManagerInstance();
    while(!ChunkManager::SHUTDOWN)
    {
        if(cm->ChunkSetupList->size() > 0)
        {
            cm->ChunkSetupMutex.lock();
            //std::cout <<"Mutex Locked by Mesher: Size:"<< cm->ChunkSetupList->size() << std::endl;
            Chunk*c = nullptr;
            if(cm->ChunkSetupList->size()>0)
            {
                c = cm->ChunkSetupList->back();
                cm->ChunkSetupList->pop_back();
            }
            //std::cout <<"Mutex Unlocked by Mesher: Size:"<< cm->ChunkSetupList->size() << std::endl;
            cm->ChunkSetupMutex.unlock();

            if(c != nullptr)
            {
                c->BuildChunkMesh();
                if(c->ShouldRender(0, new glm::vec3(), new glm::vec3()))
                {
                    cm->ChunkRenderMutex.lock();
                    cm->ChunkRenderList->push_back(c);
                    cm->ChunkRenderMutex.unlock();
                }
                else
                {
                    delete c;
                }
            }
        }
    }
}

ChunkManager::ChunkManager()
{
    ChunkLoadList = new std::vector<Chunk*>();
    ChunkSetupList = new std::vector<Chunk*>();
    ChunkVisibleList = new std::vector<Chunk*>();
    ChunkRenderList = new std::vector<Chunk*>();
    ChunkUnloadList = new std::vector<Chunk*>();
    _Debug = new DebugObject();
}

void ChunkManager::Start()
{
    _T_LoadList = new std::thread(UpdateLoadList);
    _T_LoadList2 = new std::thread(UpdateLoadList);
    _T_LoadList3 = new std::thread(UpdateLoadList);
    //_T_LoadList4 = new std::thread(UpdateLoadList);

    _T_SetupList = new std::thread(UpdateSetupList);
    _T_SetupList2 = new std::thread(UpdateSetupList);
    //_T_SetupList3 = new std::thread(UpdateSetupList);
    //_T_SetupList4 = new std::thread(UpdateSetupList);
}

void ChunkManager::Shutdown()
{
    ChunkManager::SHUTDOWN = true;
    _T_LoadList->join();
    _T_LoadList2->join();
    _T_LoadList3->join();
    //_T_LoadList4->join();

    _T_SetupList->join();
    _T_SetupList2->join();
    //_T_SetupList3->join();
    //_T_SetupList4->join();
}

void ChunkManager::Update(Camera* camera, GLuint distance)
{
    //Check for new chunks needed and Update list
    //RequestChunks(camera, distance);
    //Check list and use thread to create new chunks and meshes
    //Check if any chunks need to be rebuilt
    //Check if any chunks can be unloaded
    //Update visible list
    //Update render list
}


void ChunkManager::RequestChunks()
{
    ChunkLoadMutex.lock();
    for(int x = -10; x < 10; x++)
        for (int z = -10; z < 10; z++)
            for(int y = 1; y < 4; y++)
                ChunkLoadList->push_back(new Chunk(x,y,z));
    ChunkLoadMutex.unlock();
}



ChunkManager* ChunkManager::ChunkManagerInstance()
{
    if(_instance == nullptr)
    {
        _instance = new ChunkManager();
    }
    return _instance;
}

void ChunkManager::Render(glm::mat4* view, glm::mat4* proj, glm::mat4* mvp)
{
    VoxelRenderer::SetupShader();
    ChunkRenderMutex.lock();
    for(Chunk*c: *ChunkRenderList)
    {
        c->Render(view, proj, mvp);
    }

    if(WorldVariables::SHOW_CHUNK_BOUNDS)
    {
        for(Chunk*c: *ChunkRenderList)
        {
            _Debug->DrawDebugChunk(c->chunkX, c->chunkY, c->chunkZ, *proj, *view);
        }
    }

    ChunkRenderMutex.unlock();
}

bool ChunkManager::BlockExistsInChunk(int x, int y, int z, int cx, int cy, int cz)
{
    return GetBlockValue((cx*Chunk::CHUNK_SIZE)+x, (cy*Chunk::CHUNK_SIZE)+y, (cz*Chunk::CHUNK_SIZE)+z);
    //return false;
}


bool ChunkManager::GetBlockValue(double x, double y, double z)
{
    double px = (double)x/(Chunk::CHUNK_SIZE*20);
    double py = (double)z/(Chunk::CHUNK_SIZE*20);
    //double pz = (double)y/(num_chunks_Y*16);
    double n;
    //Wood
    //n = 20 * _perlin->noise(px, py, 0.8);
    //n = n - floor(n);
    //Standard Noise
    n = _perlin->noise( 5*px, 5*py, 0);
    n = n*300;
    return (y<n ? true:false);
}
