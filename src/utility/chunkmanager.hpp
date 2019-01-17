#ifndef CHUNKMANAGER_HPP
#define CHUNKMANAGER_HPP

#include <vector>
#include "world/chunk.hpp"
#include "camera.hpp"
#include "perlinnoise.hpp"
#include "utility/voxelrenderer.hpp"

class ChunkManager
{
public:

    //Methods
    static ChunkManager* ChunkManagerInstance();
    Chunk* GetChunk(int x, int y, int z);
    bool BlockExistsInChunk(uint x, uint y, uint z, int cx, int cy, int cz);
    void Render(glm::mat4* view, glm::mat4* proj, glm::mat4* mvp);
    bool GetBlockValue(double x, double y, double z);
    void CreateChunks();
    void BuildMeshes();
    void Update(Camera* camera, GLuint distance);

    PerlinNoise* _perlin = new PerlinNoise(10);

    //Variables
    static const uint num_chunks_X = 4;
    static const uint num_chunks_Y = 4;
    static const uint num_chunks_Z = 4;
    static const uint num_chunks = num_chunks_X * num_chunks_Y * num_chunks_Z;
    bool Shutdown = false;


    Chunk* chunks[num_chunks_X][num_chunks_Y][num_chunks_Z];

    //Constructor and Destructor
    ~ChunkManager();

private:
    //Variables
    unsigned int _ChunksLoadsPerFrame;
    unsigned int _ChunksLoaded;


    static ChunkManager* _instance;
    ChunkManager();
    glm::vec3* _ActiveChunk;
    std::vector<Chunk*>* _ChunkSetupList;
    std::vector<Chunk*>* _ChunkVisibleList;
    std::vector<Chunk*>* _ChunkRenderList;
    std::vector<Chunk*>* _ChunkLoadList;
    std::vector<Chunk*>* _ChunkUnloadList;

    //Methods
    void RequestChunks(Camera* camera, GLuint distance);


};

#endif // CHUNKMANAGER_HPP
