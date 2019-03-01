#ifndef TERRAINGENERATOR_HPP
#define TERRAINGENERATOR_HPP

#include <iostream>
#include <unordered_map>

#include "worldgenerator.hpp"
#include "worldvariables.hpp"
#include "utility/Chunk/chunk.hpp"
#include "utility/FastNoiseSIMD/FastNoiseSIMD.h"

class TerrainGenerator : public WorldGenerator
{
public:
    TerrainGenerator();
    ~TerrainGenerator();
    void GenerateChunk(Chunk* c);
};

#endif // TERRAINGENERATOR_HPP
