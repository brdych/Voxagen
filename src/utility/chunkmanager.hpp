#ifndef CHUNKMANAGER_HPP
#define CHUNKMANAGER_HPP

#include <vector>
#include "world/chunk.hpp"
#include "perlinnoise.hpp"

class ChunkManager
{
public:

    //Methods
    static ChunkManager* ChunkManagerInstance();
    Chunk* GetChunk(int x, int y, int z);
    bool BlockExistsInChunk(uint x, uint y, uint z, int cx, int cy, int cz);
    void Render();
    bool GetBlockValue(double x, double y, double z);
    void CreateChunks();
    void BuildMeshes();

    PerlinNoise* _perlin = new PerlinNoise(10);

    //Variables
    static const uint num_chunks_X = 32;
    static const uint num_chunks_Y = 1;
    static const uint num_chunks_Z = 32;
    static const uint num_chunks = num_chunks_X * num_chunks_Y * num_chunks_Z;


    Chunk* chunks[num_chunks_X][num_chunks_Y][num_chunks_Z];

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
