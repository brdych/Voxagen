#ifndef CHUNK_HPP
#define CHUNK_HPP

#include <stdio.h>
#include <iostream>
#include <vector>

#include "utility/voxelrenderer.hpp"
#include "worldvariables.hpp"

class Chunk {
public:

    //Variables
    static const int CHUNK_SIZE = 32;

    const int chunkX;
    const int chunkY;
    const int chunkZ;

    //bool* voxeldata;

    std::vector<bool> voxeldata;

    // True when Chunk has Voxel data
    bool isSetup = false;


    bool isGenerating = false;
    bool isGenerated = false;
    bool isMeshing = false;
    bool isMeshed = false;



    //Methods
    void AddCube(uint x, uint y, uint z, glm::vec3 col);
    void SetBlock(uint x, uint y, uint z, bool block);
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
