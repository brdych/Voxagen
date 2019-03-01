#ifndef CHUNKGENERATOR_HPP
#define CHUNKGENERATOR_HPP

#include <thread>
#include "utility/Generation/worldgenerator.hpp"


class ChunkGenerator
{
public:
    ChunkGenerator(WorldGenerator* generator);
    ~ChunkGenerator();
    void GenerateChunk(Chunk* c);
private:
    WorldGenerator* generator;
};

#endif // CHUNKGENERATOR_HPP
