#ifndef CHUNK_HPP
#define CHUNK_HPP

#include<stdio.h>
#include<iostream>

#include "cube.hpp"
#include "utility/voxelrenderer.hpp"
#include "worldattributes.hpp"

class Chunk {
public:

    //Variables
    static const int CHUNK_SIZE = 64;
    int chunkX;
    int chunkY;
    int chunkZ;
    bool voxeldata[CHUNK_SIZE][CHUNK_SIZE][CHUNK_SIZE];

    bool isSetup = false;
    bool needsRenderUpdate = false;
    bool needsRebuildUpdate = false;


    //Methods
    void BuildVoxelData();
    void BuildChunkMesh();
    void AddCube(uint x, uint y, uint z);
    void Render(glm::mat4* view, glm::mat4* proj, glm::mat4* mvp);
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
