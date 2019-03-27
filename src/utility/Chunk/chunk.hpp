#ifndef CHUNK_HPP
#define CHUNK_HPP

#include <stdio.h>
#include <iostream>
#include <vector>
#include <mutex>

#include "worldvariables.hpp"
#include "utility/Mesh/mesh.hpp"
#include "utility/Octree/octree.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
class Chunk {
public:

    //Variables
    inline static int CHUNK_SIZE = WorldVariables::CHUNK_SIZE;

    const int chunkX;
    const int chunkY;
    const int chunkZ;

    //std::vector<unsigned char> voxeldata;
    Octree<unsigned char,16> voxeldata = Octree<unsigned char,16>(CHUNK_SIZE,0);

    bool isGenerating = false;
    bool isGenerated = false;
    bool isMeshing = false;
    bool isMeshed = false;
    bool rebuild = false;
    bool isInitialised = false;
    bool playerData = false;
    bool forceRemesh = false;
    uint byteSize = 0;


    //Methods
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
    void RegisterUse();
    void DeregisterUse();
    uint UseStatus();

    void Init();
    void DeInit();

    //Constructor and Destructor
    Chunk(int x, int y, int z);
    ~Chunk();
    
private:
    Mesh _mesh;
    Mesh _wmesh;
    unsigned int _chunkSize = 0;
    unsigned int _numUsers = 0;
    std::mutex _userLock;
};
#endif // CHUNK_HPP
