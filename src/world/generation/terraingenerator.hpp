#ifndef TERRAINGENERATOR_HPP
#define TERRAINGENERATOR_HPP

#include <iostream>

#include "worldgenerator.hpp"
#include "worldvariables.hpp"
#include "world/chunk.hpp"
#include "utility/FastNoiseSIMD/FastNoiseSIMD.h"

class TerrainGenerator : public WorldGenerator
{
public:
    TerrainGenerator();
    ~TerrainGenerator();
    void GenerateChunk(Chunk* c);
};

#endif // TERRAINGENERATOR_HPP
