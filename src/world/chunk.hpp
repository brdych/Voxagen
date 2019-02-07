#ifndef CHUNK_HPP
#define CHUNK_HPP

#include <stdio.h>
#include <iostream>
#include <vector>

#include "utility/voxelrenderer.hpp"
#include "worldvariables.hpp"
#include "utility/mesh.hpp"
#include "glm/glm.hpp"

class Chunk {
public:

    //Variables
    inline static int CHUNK_SIZE = WorldVariables::CHUNK_SIZE;

    const int chunkX;
    const int chunkY;
    const int chunkZ;

    std::vector<bool> voxeldata;

    bool isGenerating = false;
    bool isGenerated = false;
    bool isMeshing = false;
    bool isMeshed = false;
    bool inUse = false;
    bool rebuild = false;

    //Methods
    void AddCube(uint x, uint y, uint z, glm::vec3 col);
    bool GetBlock(uint x, uint y, uint z);

    void AddBlock(uint x, uint y, uint z);
    void RemoveBlock(uint x, uint y, uint z);

    void Render(glm::mat4* view, glm::mat4* proj, glm::mat4* mvp);
    void SetMesh(Mesh m);
    Mesh* GetMesh();
    bool ShouldMesh();
    bool ShouldRender(float fov, glm::vec3* cameraFront, glm::vec3* cameraPos);
    void PrintDebug();

    //Constructor and Destructor
    Chunk(int x, int y, int z);
    ~Chunk();
    
private:
    Mesh _mesh;
    unsigned int _chunkSize;
};
#endif // CHUNK_HPP
