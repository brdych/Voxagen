#include "chunkstorage.hpp"
#include <iomanip>

int ChunkStorage::NUM_CHUNKS_STORED = 0;
uint ChunkStorage::DISTANCE;
uint ChunkStorage::RANGE;

void ChunkStorage::ChangeSize(uint distance)
{
    DISTANCE = distance;
    RANGE = (DISTANCE*2)+1;
    for(auto c: _chunksV2)
    {
        DeleteChunkV2(c);
    }
    _chunksV2.clear();
    _chunksV2.shrink_to_fit();
    _chunksV2 = std::vector<Chunk*>(RANGE*RANGE*RANGE, nullptr);
}

uint ChunkStorage::ChunkHashV2(int x, int y, int z)
{
    uint mpx = (x < 0) ? RANGE - (abs(x) % RANGE) : (x) % RANGE;
    uint mpy = (y < 0) ? RANGE - (abs(y) % RANGE) : (y) % RANGE;
    uint mpz = (z < 0) ? RANGE - (abs(z) % RANGE) : (z) % RANGE;

    /*if(mpx >= RANGE || mpy >=RANGE || mpz >=RANGE)
    {
        std::cout   << "Range: " << RANGE
                << "\nx: " << x
                << "\ny: " << y
                << "\nz: " << z
                << "\nmpx: " << mpx
                << "\nmpy: " << mpy
                << "\nmpz: " << mpz
                << std::endl;
    }*/

    if(mpx == RANGE)
        mpx = 0;
    if(mpy == RANGE)
        mpy = 0;
    if(mpz == RANGE)
        mpz = 0;

    uint ret = mpx + RANGE * (mpy + RANGE * mpz);
    if(ret >= RANGE*RANGE*RANGE)
        std::cout   << "Ret: " << ret << "Very Bad!" << std::endl;
    return ret;
}

bool ChunkStorage::ChunkExistsV2(int x, int y, int z)
{
    uint h = ChunkHashV2(x,y,z);
    auto c = _chunksV2[h];
    if(c != nullptr)
        return ((c->chunkX == x) && (c->chunkY == y) && (c->chunkZ == z));
    return false;
}

Chunk* ChunkStorage::InsertChunkV2(int x, int y, int z)
{
    uint h = ChunkHashV2(x,y,z);
    auto c = new Chunk(x,y,z);
    _chunksV2[h] = c;
    NUM_CHUNKS_STORED++;
    return c;
}

Chunk* ChunkStorage::GetChunkV2(int x, int y, int z)
{
    uint h = ChunkHashV2(x,y,z);
    auto c = _chunksV2[h];
    if(c != nullptr)
        return c;
    std::cout << "GetChunk: " << x << " " << y << " " << z << " Returned nullptr!" << std::endl;
    return nullptr;
}

void ChunkStorage::DeleteChunkV2(Chunk* c)
{
    delete c;
    WorldVariables::CHUNKSTORE_STORED_BYTES -= c->byteSize;
}

ChunkStorage::ChunkStorage()
{
    DISTANCE = 10;
    RANGE = (DISTANCE*2)+1;
    _chunksV2 = std::vector<Chunk*>(RANGE*RANGE*RANGE, nullptr);
}

ChunkStorage::~ChunkStorage()
{
    std::cout << "\t\tDeleting Chunks" << std::endl;
    for(auto c: _chunksV2) { delete c; }
    std::cout << "\t\tClearing List" << std::endl;
    _chunksV2.clear();
    _chunksV2.shrink_to_fit();
}
