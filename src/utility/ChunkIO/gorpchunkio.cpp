#include "gorpchunkio.hpp"

GorbChunkIO::GorbChunkIO()
{

}

GorbChunkIO::~GorbChunkIO()
{

}

void GorpSave(Chunk* c)
{
    std::cout << "Saving Chunk: " << c->chunkX << " " << c->chunkY << " " << c->chunkZ << std::endl;
}

int GorpLoad(Chunk* c)
{
    //return 1 if file exists
    std::cout << "Loading Chunk: " << c->chunkX << " " << c->chunkY << " " << c->chunkZ << std::endl;
    return 0;
}

void GorbChunkIO::SaveChunk(Chunk* c)
{
    std::thread t(GorpSave, c);
    t.detach();
}

int GorbChunkIO::LoadChunk(Chunk* c)
{
    return GorpLoad(c);
}
