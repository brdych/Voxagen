#ifndef CHUNK_HPP
#define CHUNK_HPP

#include<stdio.h>
#include<iostream>

#include "utility/voxelrenderer.hpp"
#include "worldvariables.hpp"

class Chunk {
public:

    //Variables
    static const int CHUNK_SIZE = 64;
    const int chunkX;
    const int chunkY;
    const int chunkZ;
    bool voxeldata[CHUNK_SIZE][CHUNK_SIZE][CHUNK_SIZE];

    //std::vector<bool> voxeldata2[CHUNK_SIZE][CHUNK_SIZE][CHUNK_SIZE];

    // True when Chunk has Voxel data
    bool isSetup = false;
    // True when Chunk has Mesh data
    bool isMeshed = false;

    //bool needsRenderUpdate = false;
    //bool needsRebuildUpdate = false;


    //Methods
    void BuildVoxelData();
    void BuildChunkMesh();
    void AddCube(uint x, uint y, uint z);
    void Render(glm::mat4* view, glm::mat4* proj, glm::mat4* mvp);
    bool ShouldMesh();
    bool ShouldRender(float fov, glm::vec3* cameraFront, glm::vec3* cameraPos);

    //Constructor and Destructor
    Chunk(int x, int y, int z);
    ~Chunk();
    
private:

    //Variables
    VoxelRenderer* _renderer;
    unsigned int _chunkSize;
};
#endif // CHUNK_HPP
