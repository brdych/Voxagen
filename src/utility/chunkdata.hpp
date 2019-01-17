#ifndef CHUNKDATA_HPP
#define CHUNKDATA_HPP

#include <vector>
#include <mutex>
#include <iostream>

#include "world/chunk.hpp"


class ChunkData
{
public:
    static ChunkData* GetChunkDataInstance();

    void AddToChunkSetupList(Chunk* c);
    std::vector<Chunk*> RemoveFromChunkSetupList();

    void AddToChunkVisibleList(Chunk* c);
    std::vector<Chunk*> RemoveFromChunkVisibleList();

    void AddToChunkRenderList(Chunk* c);
    std::vector<Chunk*> RemoveFromChunkRenderList();

    void AddToChunkLoadList(Chunk* c);
    std::vector<Chunk*> RemoveFromChunkLoadList();

    void AddToChunkUnloadList(Chunk* c);
    std::vector<Chunk*> RemoveFromChunkUnloadList();


private:
    ChunkData();
    static ChunkData* _Instance;

    std::mutex _ChunkSetupMutex;
    std::mutex _ChunkVisibleMutex;
    std::mutex _ChunkRenderMutex;
    std::mutex _ChunkLoadMutex;
    std::mutex _ChunkUnloadMutex;

    std::vector<Chunk*>* _ChunkSetupList;
    std::vector<Chunk*>* _ChunkVisibleList;
    std::vector<Chunk*>* _ChunkRenderList;
    std::vector<Chunk*>* _ChunkLoadList;
    std::vector<Chunk*>* _ChunkUnloadList;
};

#endif // CHUNKDATA_HPP
