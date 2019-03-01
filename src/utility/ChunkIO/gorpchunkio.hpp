#ifndef GORPCHUNKIO_HPP
#define GORPCHUNKIO_HPP

#include "chunkio.hpp"

class GorpChunkIO : public ChunkIO
{
public:
    GorpChunkIO();
    ~GorpChunkIO();
    void SaveChunk(Chunk* c);
    void LoadChunk(Chunk* c);
};

#endif // GORPCHUNKIO_HPP
