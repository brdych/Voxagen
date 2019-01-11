#include "chunk.hpp"
#include "utility/chunkmanager.hpp"

Chunk::Chunk(int x, int y, int z) {
    chunkX = x;
    chunkY = y;
    chunkZ = z;
    BuildVoxelData();
}

Chunk::~Chunk() {

}

void Chunk::BuildVoxelData()
{
    std::cout << "Generating Voxel Data" << std::endl;
    for(uint x = 0; x < CHUNK_SIZE; x++)
        for(uint y = 0; y < CHUNK_SIZE; y++)
            for(uint z = 0; z < CHUNK_SIZE; z++)
                voxeldata[x][y][z] = true;//(rand() < (RAND_MAX/2));
    std::cout << "Voxel Data Generated" << std::endl;
}

void Chunk::BuildChunkMesh()
{
    std::cout << "Building Chunk Mesh" << std::endl;
    for(uint z = 0; z < CHUNK_SIZE; z++)
        for(uint y = 0; y < CHUNK_SIZE; y++)
            for(uint x = 0; x < CHUNK_SIZE; x++)
                if(voxeldata[x][y][z])
                    AddCube(x,y,z);

    std::cout << _cvs->size() << std::endl;

    //VAO
    glGenVertexArrays(1, &_chunkVAO);
    glBindVertexArray(_chunkVAO);
    //VBO
    glGenBuffers(1, &_chunkVBO);
    glBindBuffer(GL_ARRAY_BUFFER, _chunkVBO);
    glBufferData(GL_ARRAY_BUFFER, _cvs->size()*sizeof(GLfloat), _cvs->data(), GL_STATIC_DRAW);
    //VAO
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);
    glEnableVertexAttribArray(0);

    std::cout << "Built Chunk Mesh" << std::endl;
}

void Chunk::Render()
{
    glBindVertexArray(_chunkVAO);
    glDrawArrays(GL_TRIANGLES, 0, _cvs->size()/3);
}

void Chunk::AddCube(uint x, uint y, uint z)
{
    ChunkManager* cm = ChunkManager::ChunkManagerInstance();
    bool adjacent_block = true;

    //std::cout << "AddCube: " << x << " " << y << " " << z <<std::endl;

    float voxel_size = WorldAttributes::BLOCK_SIZE;
    glm::vec3 v0 = glm::vec3(x-voxel_size,y-voxel_size,z-voxel_size);
    glm::vec3 v1 = glm::vec3(x-voxel_size,y-voxel_size,z+voxel_size);
    glm::vec3 v2 = glm::vec3(x+voxel_size,y-voxel_size,z-voxel_size);
    glm::vec3 v3 = glm::vec3(x+voxel_size,y-voxel_size,z+voxel_size);
    glm::vec3 v4 = glm::vec3(x-voxel_size,y+voxel_size,z-voxel_size);
    glm::vec3 v5 = glm::vec3(x-voxel_size,y+voxel_size,z+voxel_size);
    glm::vec3 v6 = glm::vec3(x+voxel_size,y+voxel_size,z-voxel_size);
    glm::vec3 v7 = glm::vec3(x+voxel_size,y+voxel_size,z+voxel_size);


    //Top
    adjacent_block = (y!=CHUNK_SIZE-1) ? voxeldata[x][y+1][z] : cm->BlockExistsInChunk(x,0,z, chunkX,chunkY+1,chunkZ);
    if(!adjacent_block)
    {
        _cvs->push_back(v4.x);
        _cvs->push_back(v4.y);
        _cvs->push_back(v4.z);

        _cvs->push_back(v7.x);
        _cvs->push_back(v7.y);
        _cvs->push_back(v7.z);

        _cvs->push_back(v6.x);
        _cvs->push_back(v6.y);
        _cvs->push_back(v6.z);

        _cvs->push_back(v4.x);
        _cvs->push_back(v4.y);
        _cvs->push_back(v4.z);

        _cvs->push_back(v5.x);
        _cvs->push_back(v5.y);
        _cvs->push_back(v5.z);

        _cvs->push_back(v7.x);
        _cvs->push_back(v7.y);
        _cvs->push_back(v7.z);
    }

    //Bottom
    adjacent_block = (y!=0) ? voxeldata[x][y-1][z] : cm->BlockExistsInChunk(x,CHUNK_SIZE-1,z, chunkX,chunkY-1,chunkZ);
    if(!adjacent_block)
    {
        _cvs->push_back(v1.x);
        _cvs->push_back(v1.y);
        _cvs->push_back(v1.z);

        _cvs->push_back(v2.x);
        _cvs->push_back(v2.y);
        _cvs->push_back(v2.z);

        _cvs->push_back(v3.x);
        _cvs->push_back(v3.y);
        _cvs->push_back(v3.z);

        _cvs->push_back(v1.x);
        _cvs->push_back(v1.y);
        _cvs->push_back(v1.z);

        _cvs->push_back(v0.x);
        _cvs->push_back(v0.y);
        _cvs->push_back(v0.z);

        _cvs->push_back(v2.x);
        _cvs->push_back(v2.y);
        _cvs->push_back(v2.z);
    }

    //Right
    adjacent_block = (x!=CHUNK_SIZE-1) ? voxeldata[x+1][y][z] : cm->BlockExistsInChunk(0,y,z, chunkX+1,chunkY,chunkZ);
    if(!adjacent_block)
    {
        _cvs->push_back(v7.x);
        _cvs->push_back(v7.y);
        _cvs->push_back(v7.z);

        _cvs->push_back(v2.x);
        _cvs->push_back(v2.y);
        _cvs->push_back(v2.z);

        _cvs->push_back(v6.x);
        _cvs->push_back(v6.y);
        _cvs->push_back(v6.z);

        _cvs->push_back(v7.x);
        _cvs->push_back(v7.y);
        _cvs->push_back(v7.z);

        _cvs->push_back(v3.x);
        _cvs->push_back(v3.y);
        _cvs->push_back(v3.z);

        _cvs->push_back(v2.x);
        _cvs->push_back(v2.y);
        _cvs->push_back(v2.z);
    }

    //Left
    adjacent_block = (x!=0) ? voxeldata[x-1][y][z] : cm->BlockExistsInChunk(CHUNK_SIZE-1,y,z, chunkX-1,chunkY,chunkZ);
    if(!adjacent_block)
    {
        _cvs->push_back(v4.x);
        _cvs->push_back(v4.y);
        _cvs->push_back(v4.z);

        _cvs->push_back(v1.x);
        _cvs->push_back(v1.y);
        _cvs->push_back(v1.z);

        _cvs->push_back(v5.x);
        _cvs->push_back(v5.y);
        _cvs->push_back(v5.z);

        _cvs->push_back(v4.x);
        _cvs->push_back(v4.y);
        _cvs->push_back(v4.z);

        _cvs->push_back(v0.x);
        _cvs->push_back(v0.y);
        _cvs->push_back(v0.z);

        _cvs->push_back(v1.x);
        _cvs->push_back(v1.y);
        _cvs->push_back(v1.z);
    }

    //Front
    adjacent_block = (z!=CHUNK_SIZE-1) ? voxeldata[x][y][z+1] : cm->BlockExistsInChunk(x,y,0, chunkX,chunkY,chunkZ+1);
    if(!adjacent_block)
    {
        _cvs->push_back(v5.x);
        _cvs->push_back(v5.y);
        _cvs->push_back(v5.z);

        _cvs->push_back(v3.x);
        _cvs->push_back(v3.y);
        _cvs->push_back(v3.z);

        _cvs->push_back(v7.x);
        _cvs->push_back(v7.y);
        _cvs->push_back(v7.z);

        _cvs->push_back(v5.x);
        _cvs->push_back(v5.y);
        _cvs->push_back(v5.z);

        _cvs->push_back(v1.x);
        _cvs->push_back(v1.y);
        _cvs->push_back(v1.z);

        _cvs->push_back(v3.x);
        _cvs->push_back(v3.y);
        _cvs->push_back(v3.z);
    }

    //Back
    adjacent_block = (z!=0) ? voxeldata[x][y][z-1] : cm->BlockExistsInChunk(x,y,CHUNK_SIZE-1, chunkX,chunkY,chunkZ-1);
    if(!adjacent_block)
    {
        _cvs->push_back(v6.x);
        _cvs->push_back(v6.y);
        _cvs->push_back(v6.z);

        _cvs->push_back(v0.x);
        _cvs->push_back(v0.y);
        _cvs->push_back(v0.z);

        _cvs->push_back(v4.x);
        _cvs->push_back(v4.y);
        _cvs->push_back(v4.z);

        _cvs->push_back(v6.x);
        _cvs->push_back(v6.y);
        _cvs->push_back(v6.z);

        _cvs->push_back(v2.x);
        _cvs->push_back(v2.y);
        _cvs->push_back(v2.z);

        _cvs->push_back(v0.x);
        _cvs->push_back(v0.y);
        _cvs->push_back(v0.z);
    }
}

/*static const GLfloat voxel_vertices[] = {
    -voxel_size,-voxel_size,-voxel_size, //BLB 0
    -voxel_size,-voxel_size, voxel_size, //BLF 1
     voxel_size,-voxel_size,-voxel_size, //BRB 2
     voxel_size,-voxel_size, voxel_size, //BRF 3
    -voxel_size, voxel_size,-voxel_size, //TLB 4
    -voxel_size, voxel_size, voxel_size, //TLF 5
     voxel_size, voxel_size,-voxel_size, //TRB 6
     voxel_size, voxel_size, voxel_size  //TRF 7
};

static const GLuint voxel_indices[] = {
    4,7,6,  //Top
    4,5,7,
    1,2,3,  //Bottom
    1,0,2,
    7,2,6,  //Right
    7,3,2,
    4,1,5,  //Left
    4,0,1,
    6,0,4,  //Back
    6,2,0,
    5,3,7,  //Front
    5,1,3
};*/

//EBO
//GLuint elementbuffer;
//glGenBuffers(1, &elementbuffer);
//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementbuffer);
//glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(voxel_indices), voxel_indices, GL_STATIC_DRAW);
//glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);
//glEnableVertexAttribArray(0);

