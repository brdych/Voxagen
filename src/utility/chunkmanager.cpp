#include "chunkmanager.hpp"
#include "math.h"

ChunkManager* ChunkManager::_instance;

ChunkManager::ChunkManager()
{
    _ChunkRenderList = new std::vector<Chunk*>();
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


void ChunkManager::RequestChunks(Camera* camera, GLuint distance)
{
    /*if(_ChunksLoaded < 10)
    {
        int chunkX = floor(camera->Position.x)/Chunk::CHUNK_SIZE;
        int chunkZ = floor(camera->Position.z)/Chunk::CHUNK_SIZE;
        int chunkY = floor(camera->Position.y)/Chunk::CHUNK_SIZE;

        for(int x = chunkX-distance; x<chunkX+distance; x++)
            for(int z = chunkZ-distance; z< chunkZ+distance; z++)
                for(int y = chunkY-distance; y < chunkY+distance; y++)
                    _ChunkLoadList->push_back(new Chunk(x,y,z));
    }
    _ChunksLoaded = 10;*/
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
    for(Chunk* c : *_ChunkRenderList)
    {
        c->Render(view, proj, mvp);
    }
}

bool ChunkManager::BlockExistsInChunk(int x, int y, int z, int cx, int cy, int cz)
{
    return GetBlockValue((cx*Chunk::CHUNK_SIZE)+x, (cy*Chunk::CHUNK_SIZE)+y, (cz*Chunk::CHUNK_SIZE)+z);
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
