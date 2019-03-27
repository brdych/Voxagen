#ifndef SAVEVTASK_HPP
#define SAVEVTASK_HPP

#include "vtask.hpp"
#include "utility/Chunk/chunk.hpp"
#include "utility/ChunkIO/filechunkio.hpp"
#include "utility/Voxagen/voxagenengine.hpp"


class SaveVTask : public VTask
{
private:
    Chunk* c;

    // VTask interface
public:
    SaveVTask(Chunk* c)
    {
        this->c = c;
    }

    void execute()
    {
        VoxagenEngine::FILE_IO->SaveChunk(c);
    }
};

#endif // SAVEVTASK_HPP
