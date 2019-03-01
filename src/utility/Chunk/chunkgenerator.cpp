#include "chunkgenerator.hpp"
#include "utility/Voxagen/voxagenengine.hpp"

void GenChunk(Chunk* c, WorldGenerator* generator)
{
    if(WorldVariables::LOAD_CHUNKS_FROM_FILE)
    {
        VoxagenEngine::FILE_IO->LoadChunk(c);
        c->isGenerated = true;
    }
    else
    {
        generator->GenerateChunk(c);
    }
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
    std::thread t(GenChunk, c, generator);
    t.detach();
}
