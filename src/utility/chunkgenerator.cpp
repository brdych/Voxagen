#include "chunkgenerator.hpp"

void GenChunk(Chunk* c, WorldGenerator* generator)
{
    generator->GenerateChunk(c);
}

ChunkGenerator::ChunkGenerator(WorldGenerator* generator)
{
    this->generator = generator;
}

ChunkGenerator::~ChunkGenerator()
{
    delete generator;
}

void ChunkGenerator::GenerateChunk(Chunk* c)
{
    std::thread t(GenChunk,c,generator);
    t.detach();
}
