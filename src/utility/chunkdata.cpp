#include "chunkdata.hpp"

ChunkData* ChunkData::_Instance;

ChunkData* ChunkData::GetChunkDataInstance()
{
    if(_Instance == nullptr)
    {
        _Instance = new ChunkData();
    }
    return _Instance;
}

ChunkData::ChunkData()
{
    _ChunkSetupList = new std::vector<Chunk*>();
    _ChunkVisibleList = new std::vector<Chunk*>();
    _ChunkRenderList = new std::vector<Chunk*>();
    _ChunkLoadList = new std::vector<Chunk*>();
    _ChunkUnloadList = new std::vector<Chunk*>();
}

void ChunkData::AddToChunkLoadList(Chunk* c)
{
    _ChunkLoadMutex.lock();
    _ChunkLoadList->push_back(c);
    _ChunkLoadMutex.unlock();
}

std::vector<Chunk*> ChunkData::RemoveFromChunkLoadList()
{
    _ChunkLoadMutex.lock();
    std::vector<Chunk*> chunks;
    for(Chunk* c : *_ChunkLoadList)
    {

    }
    _ChunkLoadMutex.unlock();
    return chunks;
}
