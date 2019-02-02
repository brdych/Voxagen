#ifndef CHUNKMANAGER_HPP
#define CHUNKMANAGER_HPP

#include <vector>
#include <mutex>
#include <thread>

#include "world/chunk.hpp"
#include "camera.hpp"
#include "perlinnoise.hpp"
#include "FastNoiseSIMD/FastNoiseSIMD.h"
#include "utility/voxelrenderer.hpp"
#include "debug.hpp"
#include "chunkgenerator.hpp"
#include "world/generation/terraingenerator.hpp"
#include "chunkmesher.hpp"


class ChunkManager
{
public:

    //Methods
    static ChunkManager* ChunkManagerInstance();

    bool BlockExistsInChunk(int x, int y, int z, int cx, int cy, int cz);
    void Render(glm::mat4* view, glm::mat4* proj, glm::mat4* mvp);
    bool GetBlockValue(double x, double y, double z);
    void CreateChunks();
    void BuildMeshes();
    void Update(Camera* camera, GLuint distance);
    void Start();
    void Shutdown();
    void RequestChunks();

    PerlinNoise* _perlin;

    //Variables
    //static const uint num_chunks_X = 4;
    //static const uint num_chunks_Y = 4;
    //static const uint num_chunks_Z = 4;
    //static const uint num_chunks = num_chunks_X * num_chunks_Y * num_chunks_Z;
    static bool SHUTDOWN;

    //Chunk* chunks[num_chunks_X][num_chunks_Y][num_chunks_Z];

    std::mutex ChunkLoadMutex;
    std::mutex ChunkSetupMutex;
    std::mutex ChunkVisibleMutex;
    std::mutex ChunkRenderMutex;
    std::mutex ChunkUnloadMutex;

    // List for requested Chunks
    std::vector<Chunk*>* ChunkLoadList;
    // List for loaded Chunks that need to be setup
    std::vector<Chunk*>* ChunkSetupList;
    // List for Chunks that are setup and may be visible
    std::vector<Chunk*>* ChunkVisibleList;
    // List for Chunks that need to be rendered
    std::vector<Chunk*>* ChunkRenderList;
    // List for Chunks that need to be unloaded/cleaned up
    std::vector<Chunk*>* ChunkUnloadList;

    //Constructor and Destructor
    ~ChunkManager();

private:
    //Variables
    unsigned int _ChunksLoadsPerFrame;
    unsigned int _ChunksLoaded;

    ChunkGenerator* _generator;
    ChunkMesher* _mesher;
    DebugObject* _debug;

    static ChunkManager* _instance;
    ChunkManager();

    //Methods
    void UpdateLoadList();
    void UpdateMeshList();
    void UpdateVisibleList(Camera* c);
    void UpdateRenderList();
    void UpdateUnloadList();
};

#endif // CHUNKMANAGER_HPP
