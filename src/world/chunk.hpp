#ifndef CHUNK_HPP
#define CHUNK_HPP

#include <stdio.h>
#include <iostream>
#include <vector>

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

    std::vector<unsigned char> voxeldata;

    bool isGenerating = false;
    bool isGenerated = false;
    bool isMeshing = false;
    bool isMeshed = false;
    bool inUse = false;
    bool rebuild = false;

    //Methods
    void AddCube(uint x, uint y, uint z, glm::vec3 col);
    unsigned char GetBlock(uint x, uint y, uint z);

    void AddBlock(uint x, uint y, uint z, unsigned char blockType);
    void RemoveBlock(uint x, uint y, uint z);

    void Render(glm::mat4* view, glm::mat4* proj, glm::mat4* mvp);
    void RenderWater(glm::mat4* view, glm::mat4* proj, glm::mat4* mvp);
    void SetMesh(Mesh m, int i);
    Mesh* GetMesh(int m);
    bool ShouldMesh();
    bool ShouldRender(float fov, glm::vec3* cameraFront, glm::vec3* cameraPos);
    void PrintDebug();

    //Constructor and Destructor
    Chunk(int x, int y, int z);
    ~Chunk();
    
private:
    Mesh _mesh;
    Mesh _wmesh;
    unsigned int _chunkSize;
};
#endif // CHUNK_HPP
