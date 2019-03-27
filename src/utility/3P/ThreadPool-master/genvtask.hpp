#ifndef GENVTASK_HPP
#define GENVTASK_HPP

#include "vtask.hpp"
#include "utility/Chunk/chunk.hpp"
#include "utility/Generation/worldgenerator.hpp"
#include "utility/Voxagen/voxagenengine.hpp"


class GenVTask : public VTask
{
private:
    Chunk* c;
    WorldGenerator* g;

    // VTask interface
public:
    GenVTask(Chunk* c, WorldGenerator* g)
    {
        this->c = c;
        this->g = g;
    }
    void execute()
    {
        if(VoxagenEngine::FILE_IO->LoadChunk(c))
        {
            c->isGenerated = true;
        }
        else
        {
            g->GenerateChunk(c);
        }
        c->byteSize = c->voxeldata.bytes();
    }
};

#endif // GENVTASK_HPP
