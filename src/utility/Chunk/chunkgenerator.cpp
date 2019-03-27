#include "chunkgenerator.hpp"
#include "utility/3P/ThreadPool-master/genvtask.hpp"
#include "utility/Voxagen/voxagenengine.hpp"

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
    VoxagenEngine::POOL->enqueue(new GenVTask(c, generator));
}
