#ifndef GORPCHUNKIO_HPP
#define GORPCHUNKIO_HPP

#include "chunkio.hpp"

class GorbChunkIO : public ChunkIO
{
public:
    GorbChunkIO();
    ~GorbChunkIO();
    void SaveChunk(Chunk* c);
    int LoadChunk(Chunk* c);
};

#endif // GORPCHUNKIO_HPP
