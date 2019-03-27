#ifndef CHUNKHELPER_HPP
#define CHUNKHELPER_HPP
#include <math.h>
#include "worldvariables.hpp"
class ChunkHelper
{
private:
static inline int CHUNK_SIZE = WorldVariables::CHUNK_SIZE;

public:

static int FloatWorldToIntWorld(float f)
{
    return floorf(f);
}

static int WorldCoordToChunk(int a)
{
    if(a >= 0)
    {
        return a/CHUNK_SIZE;
    }
    else
    {
        int tr = abs(a), rr = abs(a);
        tr = tr/CHUNK_SIZE;
        rr = rr%CHUNK_SIZE;
        if(rr > 0)
            return (tr+1)*-1;
        return tr*-1;
    }
}

static unsigned int WorldCoordToChunkCoord(int a)
{
    if(a >= 0)
    {
        int tr = a%CHUNK_SIZE;
        return static_cast<unsigned int>(tr);
    }
    else
    {
        int tr = abs(a);
        tr = tr % CHUNK_SIZE;
        tr = CHUNK_SIZE - tr;
        if(tr==CHUNK_SIZE)
            tr = 0;
        return static_cast<unsigned int>(tr);
    }
}

static unsigned int DistanceBetween(int x, int y)
{
    return static_cast<unsigned int>(abs(x-y));
}
};

#endif // CHUNKHELPER_HPP
