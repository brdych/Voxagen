#ifndef WEIRDGENERATOR_HPP
#define WEIRDGENERATOR_HPP

#include "worldgenerator.hpp"
#include "world/chunk.hpp"
#include "utility/FastNoiseSIMD/FastNoiseSIMD.h"

class WeirdGenerator : public WorldGenerator
{
public:
    WeirdGenerator();
    ~WeirdGenerator();
    void GenerateChunk(Chunk* c);
};

#endif // WEIRDGENERATOR_HPP
