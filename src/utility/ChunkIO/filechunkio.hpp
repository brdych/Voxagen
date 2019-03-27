#ifndef FILECHUNKIO_HPP
#define FILECHUNKIO_HPP

#include <iostream>
#include <fstream>
#include "utility/3P/zstr-master/src/zstr.hpp"



#include "chunkio.hpp"

class FileChunkIO : public ChunkIO
{
public:
    FileChunkIO();
    ~FileChunkIO();
    void SaveChunk(Chunk* c);
    int LoadChunk(Chunk* c);
private:
    bool FileExists(const std::string& filename);
};

#endif // FILECHUNKIO_HPP
