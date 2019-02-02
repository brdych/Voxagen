#ifndef CHUNKMESHER_HPP
#define CHUNKMESHER_HPP

#include <thread>
#include <iostream>

#include "world/chunk.hpp"

class ChunkMesher
{
public:
    ChunkMesher();
    void GenerateChunkMesh(Chunk* c);
};

#endif // CHUNKMESHER_HPP
