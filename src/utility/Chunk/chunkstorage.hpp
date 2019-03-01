#ifndef CHUNKSTORAGE_HPP
#define CHUNKSTORAGE_HPP

#define MAX2(x,y) ((x>y)?x:y)
#define MAX3(x,y,z) ((x>y)?MAX2(x,z):MAX2(y,z))

#include <unordered_map>
#include <mutex>

#include "utility/Chunk/chunk.hpp"

class ChunkStorage
{
public:
    ChunkStorage();
    ~ChunkStorage();

    int ChunkHashV2(int x, int y, int z);
    void ChangeSize(uint distance);
    void DeleteChunkV2(Chunk* c);
    bool ChunkExistsV2(int h);
    bool ChunkExistsV2(int x, int y, int z);

    Chunk* InsertChunkV2(int h);
    Chunk* InsertChunkV2(int x, int y, int z);
    Chunk* GetChunkV2(int h);
    Chunk* GetChunkV2(int x, int y, int z);

    static int NUM_CHUNKS_STORED;
    static uint DISTANCE;
    static uint RANGE;

private:
    std::vector<Chunk*> _chunksV2;
};

#endif // CHUNKSTORAGE_HPP
