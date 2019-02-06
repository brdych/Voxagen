#include "chunkmesher.hpp"

void GenChunk(Chunk* c)
{
    WorldVariables::MESH_LOCK.lock();
    WorldVariables::NUM_MESH_THREADS++;
    WorldVariables::MESH_LOCK.unlock();


    uint CHUNK_SIZE = WorldVariables::CHUNK_SIZE;
    for(uint x = 0; x < CHUNK_SIZE; x++)
        for(uint z = 0; z < CHUNK_SIZE; z++)
            for(uint y = 0; y < CHUNK_SIZE; y++)
                if(c->voxeldata[x + CHUNK_SIZE * (y + CHUNK_SIZE * z)])
                {
                    int ya = y + (c->chunkY*CHUNK_SIZE);
                    glm::vec3 col;
                    if(ya < 100)        {col = glm::vec3(0.0,0.8,1.0);}
                    else if(ya < 130)   {col = glm::vec3(0.8,0.8,0.4);}
                    else if (ya < 200)  {col = glm::vec3(0.1,0.4,0.1);}
                    else if (ya < 230)  {col = glm::vec3(0.5,0.4,0.3);}
                    else                {col = glm::vec3(1,1,1);}
                    //std::cout << "Adding Cube for Chunk: " << c->chunkX << " " << c->chunkY << " " << c->chunkZ << std::endl;
                    c->AddCube(x,y,z,col);
                }
    c->isMeshed = true;

    WorldVariables::MESH_LOCK.lock();
    WorldVariables::NUM_MESH_THREADS--;
    WorldVariables::MESH_LOCK.unlock();
}

ChunkMesher::ChunkMesher()
{

}

void ChunkMesher::GenerateChunkMesh(Chunk* c)
{
    std::thread t(GenChunk,c);
    t.detach();
}
