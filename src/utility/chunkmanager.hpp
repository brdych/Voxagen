#ifndef CHUNKMANAGER_HPP
#define CHUNKMANAGER_HPP

#include <vector>
#include <mutex>
#include <thread>
#include <chrono>
#include <math.h>

#include "world/chunk.hpp"
#include "camera.hpp"
#include "debug.hpp"
#include "world/generation/terraingenerator.hpp"
#include "world/generation/weirdgenerator.hpp"
#include "chunkgenerator.hpp"
#include "chunkmesher.hpp"
#include "chunkstorage.hpp"


class ChunkManager
{
public:

    // Methods
    ~ChunkManager();
    ChunkManager();
    void Render(glm::mat4* view, glm::mat4* proj, glm::mat4* mvp);
    void Update(Camera* camera, GLuint distance);
    void ClearRenderList();

    // Variables
    // List for requested Chunks
    std::vector<Chunk*> ChunkLoadList;
    // List for loaded Chunks that need to be Meshed
    std::vector<Chunk*> ChunkMeshList;
    // List for Chunks that are Meshed and may be visible
    std::vector<Chunk*> ChunkVisibleList;
    // List for Chunks that need to be rendered
    std::vector<Chunk*> ChunkRenderList;
    // List for Chunks that need are Empty
    std::vector<Chunk*> ChunkEmptyList;
    // List for Chunks that need to be unloaded/deleted
    std::vector<Chunk*> ChunkUnloadList;
    // Map of all active chunks
    ChunkStorage ChunkStore = ChunkStorage();

private:
    // Variables
    ChunkGenerator _generator = ChunkGenerator(new TerrainGenerator());
    ChunkMesher _mesher;
    DebugObject _debug;
    glm::vec3 _curChunk = glm::vec3(0,0,0);

    // Methods
    void RequestChunks(Camera* c);
    void UpdateLoadList();
    void UpdateMeshList();
    void UpdateVisibleList(Camera* c);
    void UpdateEmptyList();
    void UpdateUnloadList();
    bool ChunkInViewDistance(Chunk* c);
    bool ChunkInViewDistance(int x, int y, int z);
};

#endif // CHUNKMANAGER_HPP
