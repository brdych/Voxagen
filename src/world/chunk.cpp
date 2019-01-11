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
    ChunkManager* cm = ChunkManager::ChunkManagerInstance();
    int cx = chunkX*CHUNK_SIZE, cy = chunkY*CHUNK_SIZE, cz = chunkZ * CHUNK_SIZE;
    //std::cout << "Generating Voxel Data" << std::endl;
    for(uint x = 0; x < CHUNK_SIZE; x++)
        for(uint z = 0; z < CHUNK_SIZE; z++)
            for(uint y = 0; y < CHUNK_SIZE; y++)
            {

                //voxeldata[x][y][z] = (rand() < (RAND_MAX/2));
                //voxeldata[x][y][z] = true;
                voxeldata[x][y][z] = cm->GetBlockValue(x+cx,y+cy,z+cz);
            }
    //std::cout << "Voxel Data Generated" << std::endl;
}

void Chunk::BuildChunkMesh()
{
    //std::cout << "Building Chunk Mesh" << std::endl;
    for(uint z = 0; z < CHUNK_SIZE; z++)
        for(uint y = 0; y < CHUNK_SIZE; y++)
            for(uint x = 0; x < CHUNK_SIZE; x++)
                if(voxeldata[x][y][z])
                    AddCube(x,y,z);

    //std::cout << _cvs->size() << std::endl;

    //VAO
    glGenVertexArrays(1, &_chunkVAO);
    glBindVertexArray(_chunkVAO);
    //VBO
    glGenBuffers(1, &_chunkVBO);
    glBindBuffer(GL_ARRAY_BUFFER, _chunkVBO);
    glBufferData(GL_ARRAY_BUFFER, _cvs->size()*sizeof(GLfloat), _cvs->data(), GL_STATIC_DRAW);
    //VAO
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), nullptr);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 1, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (GLvoid*)(3*sizeof(GLfloat)));
    glEnableVertexAttribArray(1);

    //std::cout << "Built Chunk Mesh" << std::endl;
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

    //0 = 0 1 0
    //1 = 0 -1 0
    //2 = 1 0 0
    //3 = -1 0 0
    //4 = 0 0 1
    //5 = 0 0 -1

    //Top
    adjacent_block = (y!=CHUNK_SIZE-1) ? voxeldata[x][y+1][z] : cm->BlockExistsInChunk(x,0,z, chunkX,chunkY+1,chunkZ);
    if(!adjacent_block)
    {
        _cvs->push_back(v4.x);
        _cvs->push_back(v4.y);
        _cvs->push_back(v4.z);
        _cvs->push_back(0.0f);

        _cvs->push_back(v7.x);
        _cvs->push_back(v7.y);
        _cvs->push_back(v7.z);
        _cvs->push_back(0.0f);

        _cvs->push_back(v6.x);
        _cvs->push_back(v6.y);
        _cvs->push_back(v6.z);
        _cvs->push_back(0.0f);

        _cvs->push_back(v4.x);
        _cvs->push_back(v4.y);
        _cvs->push_back(v4.z);
        _cvs->push_back(0.0f);

        _cvs->push_back(v5.x);
        _cvs->push_back(v5.y);
        _cvs->push_back(v5.z);
        _cvs->push_back(0.0f);

        _cvs->push_back(v7.x);
        _cvs->push_back(v7.y);
        _cvs->push_back(v7.z);
        _cvs->push_back(0.0f);
    }

    //Bottom
    adjacent_block = (y!=0) ? voxeldata[x][y-1][z] : cm->BlockExistsInChunk(x,CHUNK_SIZE-1,z, chunkX,chunkY-1,chunkZ);
    if(!adjacent_block)
    {
        _cvs->push_back(v1.x);
        _cvs->push_back(v1.y);
        _cvs->push_back(v1.z);
        _cvs->push_back(1.0f);

        _cvs->push_back(v2.x);
        _cvs->push_back(v2.y);
        _cvs->push_back(v2.z);
        _cvs->push_back(1.0f);

        _cvs->push_back(v3.x);
        _cvs->push_back(v3.y);
        _cvs->push_back(v3.z);
        _cvs->push_back(1.0f);

        _cvs->push_back(v1.x);
        _cvs->push_back(v1.y);
        _cvs->push_back(v1.z);
        _cvs->push_back(1.0f);

        _cvs->push_back(v0.x);
        _cvs->push_back(v0.y);
        _cvs->push_back(v0.z);
        _cvs->push_back(1.0f);

        _cvs->push_back(v2.x);
        _cvs->push_back(v2.y);
        _cvs->push_back(v2.z);
        _cvs->push_back(1.0f);
    }

    //Right
    adjacent_block = (x!=CHUNK_SIZE-1) ? voxeldata[x+1][y][z] : cm->BlockExistsInChunk(0,y,z, chunkX+1,chunkY,chunkZ);
    if(!adjacent_block)
    {
        _cvs->push_back(v7.x);
        _cvs->push_back(v7.y);
        _cvs->push_back(v7.z);
        _cvs->push_back(2.0f);

        _cvs->push_back(v2.x);
        _cvs->push_back(v2.y);
        _cvs->push_back(v2.z);
        _cvs->push_back(2.0f);

        _cvs->push_back(v6.x);
        _cvs->push_back(v6.y);
        _cvs->push_back(v6.z);
        _cvs->push_back(2.0f);

        _cvs->push_back(v7.x);
        _cvs->push_back(v7.y);
        _cvs->push_back(v7.z);
        _cvs->push_back(2.0f);

        _cvs->push_back(v3.x);
        _cvs->push_back(v3.y);
        _cvs->push_back(v3.z);
        _cvs->push_back(2.0f);

        _cvs->push_back(v2.x);
        _cvs->push_back(v2.y);
        _cvs->push_back(v2.z);
        _cvs->push_back(2.0f);
    }

    //Left
    adjacent_block = (x!=0) ? voxeldata[x-1][y][z] : cm->BlockExistsInChunk(CHUNK_SIZE-1,y,z, chunkX-1,chunkY,chunkZ);
    if(!adjacent_block)
    {
        _cvs->push_back(v4.x);
        _cvs->push_back(v4.y);
        _cvs->push_back(v4.z);
        _cvs->push_back(3.0f);

        _cvs->push_back(v1.x);
        _cvs->push_back(v1.y);
        _cvs->push_back(v1.z);
        _cvs->push_back(3.0f);

        _cvs->push_back(v5.x);
        _cvs->push_back(v5.y);
        _cvs->push_back(v5.z);
        _cvs->push_back(3.0f);

        _cvs->push_back(v4.x);
        _cvs->push_back(v4.y);
        _cvs->push_back(v4.z);
        _cvs->push_back(3.0f);

        _cvs->push_back(v0.x);
        _cvs->push_back(v0.y);
        _cvs->push_back(v0.z);
        _cvs->push_back(3.0f);

        _cvs->push_back(v1.x);
        _cvs->push_back(v1.y);
        _cvs->push_back(v1.z);
        _cvs->push_back(3.0f);
    }

    //Front
    adjacent_block = (z!=CHUNK_SIZE-1) ? voxeldata[x][y][z+1] : cm->BlockExistsInChunk(x,y,0, chunkX,chunkY,chunkZ+1);
    if(!adjacent_block)
    {
        _cvs->push_back(v5.x);
        _cvs->push_back(v5.y);
        _cvs->push_back(v5.z);
        _cvs->push_back(4.0f);

        _cvs->push_back(v3.x);
        _cvs->push_back(v3.y);
        _cvs->push_back(v3.z);
        _cvs->push_back(4.0f);

        _cvs->push_back(v7.x);
        _cvs->push_back(v7.y);
        _cvs->push_back(v7.z);
        _cvs->push_back(4.0f);

        _cvs->push_back(v5.x);
        _cvs->push_back(v5.y);
        _cvs->push_back(v5.z);
        _cvs->push_back(4.0f);

        _cvs->push_back(v1.x);
        _cvs->push_back(v1.y);
        _cvs->push_back(v1.z);
        _cvs->push_back(4.0f);

        _cvs->push_back(v3.x);
        _cvs->push_back(v3.y);
        _cvs->push_back(v3.z);
        _cvs->push_back(4.0f);
    }

    //Back
    adjacent_block = (z!=0) ? voxeldata[x][y][z-1] : cm->BlockExistsInChunk(x,y,CHUNK_SIZE-1, chunkX,chunkY,chunkZ-1);
    if(!adjacent_block)
    {
        _cvs->push_back(v6.x);
        _cvs->push_back(v6.y);
        _cvs->push_back(v6.z);
        _cvs->push_back(5.0f);

        _cvs->push_back(v0.x);
        _cvs->push_back(v0.y);
        _cvs->push_back(v0.z);
        _cvs->push_back(5.0f);

        _cvs->push_back(v4.x);
        _cvs->push_back(v4.y);
        _cvs->push_back(v4.z);
        _cvs->push_back(5.0f);

        _cvs->push_back(v6.x);
        _cvs->push_back(v6.y);
        _cvs->push_back(v6.z);
        _cvs->push_back(5.0f);

        _cvs->push_back(v2.x);
        _cvs->push_back(v2.y);
        _cvs->push_back(v2.z);
        _cvs->push_back(5.0f);

        _cvs->push_back(v0.x);
        _cvs->push_back(v0.y);
        _cvs->push_back(v0.z);
        _cvs->push_back(5.0f);
    }
}

