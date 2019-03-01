#ifndef CHUNKMESHER_HPP
#define CHUNKMESHER_HPP

#include <thread>
#include <iostream>

#include "utility/Chunk/chunk.hpp"
#include "utility/Mesh/mesh.hpp"

class ChunkMesher
{
public:
    ChunkMesher();
    void GenerateChunkMesh(Chunk* c, Chunk* top, Chunk* bottom, Chunk* right, Chunk* left, Chunk* front, Chunk* back);
private:
    uint CHUNK_SIZE = WorldVariables::CHUNK_SIZE;
    float VOXEL_SIZE = WorldVariables::BLOCK_SIZE;
};

#endif // CHUNKMESHER_HPP
