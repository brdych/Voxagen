#ifndef CHUNKMANAGER_HPP
#define CHUNKMANAGER_HPP

#include <vector>
#include "world/chunk.hpp"

class ChunkManager
{
public:

    //Methods
    static ChunkManager* ChunkManagerInstance();
    Chunk* GetChunk(int x, int y, int z);
    bool BlockExistsInChunk(uint x, uint y, uint z, int cx, int cy, int cz);
    void Render();
    void CreateChunks();
    void BuildMeshes();

    //Variables
    static const uint num_chunks = 16;
    Chunk* chunks[num_chunks][num_chunks][num_chunks];

    //Constructor and Destructor
    ~ChunkManager();

private:
    //Variables
    static ChunkManager* _instance;
    ChunkManager();

    //std::vector<Chunk> _ChunkSetupList;
    //std::vector<Chunk> _ChunkVisibleList;
    //std::vector<Chunk> _ChunkRenderList;
    //std::vector<Chunk> _ChunkLoadList;
    //std::vector<Chunk> _ChunkUnloadList;
};

#endif // CHUNKMANAGER_HPP
