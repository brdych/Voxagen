#include "chunkstorage.hpp"

int ChunkStorage::NUM_CHUNKS_STORED = 0;

ChunkStorage::ChunkStorage()
{
    _chunks = new std::unordered_map<std::string, Chunk*>();
}

ChunkStorage::~ChunkStorage()
{
    std::cout << "\t\tDeleting Chunks" << std::endl;
    for(std::pair<std::string, Chunk*> c: *_chunks) { delete c.second; }
    std::cout << "\t\tClearing List" << std::endl;
    _chunks->clear();
    std::cout << "\t\tDeleting List" << std::endl;
    delete _chunks;
}

Chunk* ChunkStorage::InsertChunk(int x, int y, int z)
{
    Chunk* c = new Chunk(x,y,z);
    _mapLock.lock();
        _chunks->emplace(ChunkHash(x,y,z),c);
    _mapLock.unlock();
    NUM_CHUNKS_STORED++;
    return c;
}

Chunk* ChunkStorage::GetChunk(int x, int y, int z)
{
    std::string h = ChunkHash(x,y,z);
    Chunk* c = _chunks->at(h);
    return c;
}

Chunk* ChunkStorage::GetChunk(std::string h)
{
    Chunk* c = _chunks->at(h);
    return c;
}

void ChunkStorage::DeleteChunk(int x, int y, int z)
{
    std::string h = ChunkHash(x,y,z);
    if(ChunkExists(h))
    {
        _mapLock.lock();
            delete _chunks->at(h);
            _chunks->erase(_chunks->find(h));
        _mapLock.unlock();
        NUM_CHUNKS_STORED--;
    }
}

void ChunkStorage::DeleteChunk(std::string h)
{
    if(ChunkExists(h))
    {
        _mapLock.lock();
            delete _chunks->at(h);
            _chunks->erase(_chunks->find(h));
        _mapLock.unlock();
        NUM_CHUNKS_STORED--;
    }
}

bool ChunkStorage::ChunkExists(int x, int y, int z)
{
    _mapLock.lock();
        bool val = _chunks->find(ChunkHash(x,y,z)) != _chunks->end();
    _mapLock.unlock();
    return val;
}

bool ChunkStorage::ChunkExists(std::string h)
{
    _mapLock.lock();
        bool val = _chunks->find(h) != _chunks->end();
    _mapLock.unlock();
    return val;
}

std::string ChunkStorage::ChunkHash(int x, int y, int z)
{
    return std::to_string(x) + "|" + std::to_string(y) + "|" + std::to_string(z);
}
