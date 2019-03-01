#include "gorpchunkio.hpp"

GorpChunkIO::GorpChunkIO()
{

}

GorpChunkIO::~GorpChunkIO()
{

}

void GorpSave(Chunk* c)
{
    std::cout << "Saving Chunk: " << c->chunkX << " " << c->chunkY << " " << c->chunkZ << std::endl;
}

void GorpLoad(Chunk* c)
{
    std::cout << "Loading Chunk: " << c->chunkX << " " << c->chunkY << " " << c->chunkZ << std::endl;
}

void GorpChunkIO::SaveChunk(Chunk* c)
{
    std::thread t(GorpSave, c);
    t.detach();
}

void GorpChunkIO::LoadChunk(Chunk* c)
{
    std::thread t(GorpLoad, c);
    t.detach();
}
