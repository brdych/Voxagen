#ifndef WORLDGENERATOR_HPP
#define WORLDGENERATOR_HPP

#include "world/chunk.hpp"

class WorldGenerator
{
public:
    WorldGenerator()
    {

    }
    virtual ~WorldGenerator()
    {

    }
    virtual void GenerateChunk(Chunk* c) = 0;
};

#endif // WORLDGENERATOR_HPP
