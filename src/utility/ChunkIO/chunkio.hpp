#ifndef CHUNKIO_HPP
#define CHUNKIO_HPP

#include <thread>
#include "utility/Chunk/chunk.hpp"

class ChunkIO
{
public:
    ChunkIO(){}
    virtual ~ChunkIO(){}
    virtual void SaveChunk(Chunk* c) = 0;
    virtual int LoadChunk(Chunk* c) = 0;
};

#endif // CHUNKIO_HPP
