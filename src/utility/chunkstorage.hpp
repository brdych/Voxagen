#ifndef CHUNKSTORAGE_HPP
#define CHUNKSTORAGE_HPP

#define MAX2(x,y) ((x>y)?x:y)
#define MAX3(x,y,z) ((x>y)?MAX2(x,z):MAX2(y,z))

#include <unordered_map>
#include <mutex>

#include "world/chunk.hpp"

class ChunkStorage
{
public:
    ChunkStorage();
    ~ChunkStorage();

    bool ChunkExists(int x, int y, int z);
    void DeleteChunk(int x, int y, int z);
    Chunk* GetChunk(int x, int y, int z);
    Chunk* InsertChunk(int x, int y, int z);
    static int NUM_CHUNKS_STORED;

private:
    std::unordered_map<std::string, Chunk*>* _chunks;
    std::string ChunkHash(int x, int y, int z);
    std::mutex _mapLock;
};

#endif // CHUNKSTORAGE_HPP
