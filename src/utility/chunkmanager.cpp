#include "chunkmanager.hpp"
#include "math.h"

ChunkManager* ChunkManager::_instance;

ChunkManager::ChunkManager()
{

}

void ChunkManager::CreateChunks()
{
    for(uint x=0; x<num_chunks_X; x++)
        for(uint y=0; y<num_chunks_Y; y++)
            for(uint z=0; z<num_chunks_Z; z++)
                chunks[x][y][z] = new Chunk(x,y,z);
}

void ChunkManager::BuildMeshes()
{
    for(uint x=0; x<num_chunks_X; x++)
        for(uint y=0; y<num_chunks_Y; y++)
            for(uint z=0; z<num_chunks_Z; z++)
                chunks[x][y][z]->BuildChunkMesh();
}

ChunkManager* ChunkManager::ChunkManagerInstance()
{
    //std::cout << "Request: ChunkManager Instance" <<std::endl;
    if(_instance == nullptr)
    {
        std::cout << "- Creating New ChunkManager Instance" <<std::endl;
        _instance = new ChunkManager();
    }
    return _instance;
}

void ChunkManager::Render()
{

}

Chunk* ChunkManager::GetChunk(int x, int y, int z)
{
    //std::cout << "GetChunk(" << x << " " << y << " " << z << ")" << std::endl;
    if(x<num_chunks_X&&y<num_chunks_Y&&z<num_chunks_Z&&x>=0&&y>=0&&z>=0)
    {
        //std::cout << " - Allowed" << std::endl;
        return chunks[x][y][z];
    }
    return nullptr;
}

bool ChunkManager::BlockExistsInChunk(uint x, uint y, uint z, int cx, int cy, int cz)
{
    //std::cout << "BlockExistsInChunk(" << x << " " << y << " " << z << ")" << std::endl;
    Chunk* c = GetChunk(cx,cy,cz);
    if(c == nullptr)
    {
        return false;
    }
    return c->voxeldata[x][y][z];
}


bool ChunkManager::GetBlockValue(double x, double y, double z)
{
    double px = (double)x/(num_chunks_X*16);
    double py = (double)z/(num_chunks_Z*16);
    double n;

    // Typical Perlin noise
    n = _perlin->noise(10 * px, 10 * py, 0.8);

    // Wood like structure
    //n = 20 * _perlin->noise(px, py, 0.8);
    //n = n - floor(n);

    // Map the values to the [0, 255] interval, for simplicity we use
    // tones of grey
    n = (16)*n;

    std::cout << "y= " << y << " n= "  << n << std::endl;
    return (y<n ? true:false);
}
