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
    static const int CHUNK_SIZE = 16;
    int chunkX;
    int chunkY;
    int chunkZ;
    bool voxeldata[CHUNK_SIZE][CHUNK_SIZE][CHUNK_SIZE];


    //Methods
    void BuildVoxelData();
    void BuildChunkMesh();
    void AddCube(uint x, uint y, uint z);
    void Render();
    bool ShouldRender();

    //Constructor and Destructor
    Chunk(int x, int y, int z);
    ~Chunk();
    
private:

    //Variables
    VoxelRenderer _renderer;
    std::vector<GLfloat>* _cvs = new std::vector<GLfloat>();
    GLuint _chunkVAO;
    GLuint _chunkVBO;
};
#endif // CHUNK_HPP
