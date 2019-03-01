#ifndef FILECHUNKIO_HPP
#define FILECHUNKIO_HPP

#include <iostream>
#include <fstream>

#include "chunkio.hpp"

class FileChunkIO : public ChunkIO
{
public:
    FileChunkIO();
    ~FileChunkIO();
    void SaveChunk(Chunk* c);
    void LoadChunk(Chunk* c);
};

#endif // FILECHUNKIO_HPP
