#include "filechunkio.hpp"

FileChunkIO::FileChunkIO()
{

}

FileChunkIO::~FileChunkIO()
{

}

void FileSave(Chunk* c)
{
    std::ofstream myfile;
    myfile.open (std::string("savedata/" + std::to_string(c->chunkX) + "." + std::to_string(c->chunkY) + "." + std::to_string(c->chunkZ) + ".vxg"));
    for(int z = 0; z<WorldVariables::CHUNK_SIZE; z++)
    {
        for(int y = 0; y<WorldVariables::CHUNK_SIZE; y++)
        {
            for(int x = 0; x<WorldVariables::CHUNK_SIZE; x++)
            {
                myfile << c->GetBlock(x,y,z);
            }
        }
    }
    myfile.close();
}

void FileLoad(Chunk* c)
{
    std::ifstream myfile;
    myfile.open ("savedata/" + std::to_string(c->chunkX) + "." + std::to_string(c->chunkY) + "." + std::to_string(c->chunkZ) + ".vxg",std::ifstream::binary);
    myfile.seekg (0, myfile.end);
    int length = myfile.tellg();
    myfile.seekg (0, myfile.beg);

    if(length > 0)
    {
        char* buffer = new char[length];
        myfile.read(buffer, length);
        myfile.close();

        int i = 0;
        for(int z = 0; z<WorldVariables::CHUNK_SIZE; z++)
        {
            for(int y = 0; y<WorldVariables::CHUNK_SIZE; y++)
            {
                for(int x = 0; x<WorldVariables::CHUNK_SIZE; x++)
                {
                    if(buffer[i] != 0)
                    {
                        c->AddBlock(x,y,z, buffer[i]);
                    }
                    i++;
                }
            }
        }
        delete[] buffer;
    }
    else
    {
        myfile.close();
    }
}

void FileChunkIO::SaveChunk(Chunk* c)
{
    std::thread t(FileSave, c);
    t.detach();
}

void FileChunkIO::LoadChunk(Chunk* c)
{
    FileLoad(c);
}
