#include "chunkmanager.hpp"

ChunkManager* ChunkManager::_instance;

ChunkManager::ChunkManager()
{

}

void ChunkManager::CreateChunks()
{
    for(uint x=0; x<num_chunks; x++)
        for(uint y=0; y<num_chunks; y++)
            for(uint z=0; z<num_chunks; z++)
                chunks[x][y][z] = new Chunk(x,y,z);
}

void ChunkManager::BuildMeshes()
{
    for(uint x=0; x<num_chunks; x++)
        for(uint y=0; y<num_chunks; y++)
            for(uint z=0; z<num_chunks; z++)
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
    if(x<num_chunks&&y<num_chunks&&z<num_chunks&&x>=0&&y>=0&&z>=0)
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
