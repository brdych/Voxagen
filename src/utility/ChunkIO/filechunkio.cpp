#include "filechunkio.hpp"
#include <sys/stat.h>

FileChunkIO::FileChunkIO()
{

}

FileChunkIO::~FileChunkIO()
{

}

bool FileChunkIO::FileExists (const std::string& filename)
{
  struct stat buffer;
  return (stat (filename.c_str(), &buffer) == 0);
}

void FileChunkIO::SaveChunk(Chunk* c)
{
    std::string filename = "savedata/" + std::to_string(c->chunkX) + "." + std::to_string(c->chunkY) + "." + std::to_string(c->chunkZ) + ".vxg";
    zstr::ofstream zfilex(filename);
    for(uint z = 0; z<WorldVariables::CHUNK_SIZE; z++)
    {
        for(uint y = 0; y<WorldVariables::CHUNK_SIZE; y++)
        {
            for(uint x = 0; x<WorldVariables::CHUNK_SIZE; x++)
            {
                zfilex << c->GetBlock(x,y,z);
            }
        }
    }
    zfilex.flush();
}

int FileChunkIO::LoadChunk(Chunk* c)
{
    uint CHUNK_SIZE = WorldVariables::CHUNK_SIZE;
    std::string filename = "savedata/" + std::to_string(c->chunkX) + "." + std::to_string(c->chunkY) + "." + std::to_string(c->chunkZ) + ".vxg";
    if(FileExists(filename))
    {
        zstr::ifstream zfilex(filename);
        c->Init();
        for(uint z = 0; z < CHUNK_SIZE; z++)
        {
            for(uint y = 0; y < CHUNK_SIZE; y++)
            {
                for(uint x = 0; x < CHUNK_SIZE; x++)
                {
                    if(zfilex.eof())
                    {
                        std::cout << "Bad Chunk: " << std::to_string(c->chunkX) << "." << std::to_string(c->chunkY) << "." << std::to_string(c->chunkZ) << std::endl;
                        return 0;
                    }
                    else
                    {
                        unsigned char b = static_cast<unsigned char>(zfilex.get());
                        if(b!=0) { c->AddBlock(x,y,z, b); }
                    }
                }
            }
        }
        return 1;
    }
    else
    {
        return 0;
    }
}
