#include "chunk.hpp"
#include "utility/chunkmanager.hpp"
#include "glm/glm.hpp"
#include "chrono"

Chunk::Chunk(int x, int y, int z) : chunkX(x), chunkY(y), chunkZ(z) {
    _renderer = new VoxelRenderer();
    _chunkSize = 0;
}

Chunk::~Chunk() {
    std::cout << "DESTRUCTOR CALLED! uh oh.." << std::endl;
}

bool Chunk::ShouldRender(float fov, glm::vec3* cameraFront, glm::vec3* cameraPos)
{
    if(_chunkSize < 1)
        return false;
    if(!_renderer->ShouldRender())
        return false;
    return true;
}

bool Chunk::ShouldMesh()
{
    return _chunkSize > 0;
}

void Chunk::BuildVoxelData()
{
    auto begin = std::chrono::high_resolution_clock::now();
    ChunkManager* cm = ChunkManager::ChunkManagerInstance();
    int cx = chunkX*CHUNK_SIZE, cy = chunkY*CHUNK_SIZE, cz = chunkZ * CHUNK_SIZE;
    //std::cout << "Generating Voxel Data" << std::endl;
    for(int x = 0; x < CHUNK_SIZE; x++)
        for(int z = 0; z < CHUNK_SIZE; z++)
            for(int y = 0; y < CHUNK_SIZE; y++)
            {
                bool block;
                block = voxeldata[x][y][z] = cm->GetBlockValue(x+cx,y+cy,z+cz);
                //block = voxeldata[x][y][z] = (rand() < (RAND_MAX/2));
                //block = voxeldata[x][y][z] = true;
                _chunkSize = (block) ? _chunkSize+1 : _chunkSize;
            }
    auto end = std::chrono::high_resolution_clock::now();
    auto dur = end - begin;
    auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(dur).count();
    std::cout << "Chunk Data("<<_chunkSize<<") For Chunk: "<<chunkX<<" "<<chunkY<<" "<<chunkZ<<" took: "<< ms << std::endl;
}

void Chunk::BuildChunkMesh()
{
    if(ShouldMesh())
    {
        auto begin = std::chrono::high_resolution_clock::now();
        //_renderer->StartMesh();
        for(uint z = 0; z < CHUNK_SIZE; z++)
            for(uint y = 0; y < CHUNK_SIZE; y++)
                for(uint x = 0; x < CHUNK_SIZE; x++)
                    if(voxeldata[x][y][z])
                    {
                        //std::cout << "Mesh For Block: "<<x<<" "<<y<<" "<<z<<" generating" << std::endl;
                        AddCube(x,y,z);
                    }
        //_renderer->FinishMesh();
        auto end = std::chrono::high_resolution_clock::now();
        auto dur = end - begin;
        auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(dur).count();
        std::cout << "Chunk Mesh For Chunk: "<<chunkX<<" "<<chunkY<<" "<<chunkZ<<" took: "<< ms << std::endl << std::endl;
    }
}

void Chunk::Render(glm::mat4* view, glm::mat4* proj, glm::mat4* mvp)
{
    if(_chunkSize > 0)
    {
        *mvp = (*proj) * (*view) * glm::translate(glm::mat4(1), glm::vec3(chunkX*Chunk::CHUNK_SIZE,chunkY*Chunk::CHUNK_SIZE,chunkZ*Chunk::CHUNK_SIZE));
        _renderer->Render(*mvp);
    }
}

void Chunk::AddCube(uint x, uint y, uint z)
{
    ChunkManager* cm = ChunkManager::ChunkManagerInstance();

    //std::cout << "AddCube: " << x << " " << y << " " << z <<std::endl;

    float voxel_size = WorldVariables::BLOCK_SIZE;
    glm::vec3 v0 = glm::vec3(x-voxel_size,y-voxel_size,z-voxel_size);
    glm::vec3 v1 = glm::vec3(x-voxel_size,y-voxel_size,z+voxel_size);
    glm::vec3 v2 = glm::vec3(x+voxel_size,y-voxel_size,z-voxel_size);
    glm::vec3 v3 = glm::vec3(x+voxel_size,y-voxel_size,z+voxel_size);
    glm::vec3 v4 = glm::vec3(x-voxel_size,y+voxel_size,z-voxel_size);
    glm::vec3 v5 = glm::vec3(x-voxel_size,y+voxel_size,z+voxel_size);
    glm::vec3 v6 = glm::vec3(x+voxel_size,y+voxel_size,z-voxel_size);
    glm::vec3 v7 = glm::vec3(x+voxel_size,y+voxel_size,z+voxel_size);

    //std::cout << "Init" <<std::endl;

    //Top
    if(! ((y!=CHUNK_SIZE-1) ? voxeldata[x][y+1][z] : cm->BlockExistsInChunk(x,0,z, chunkX,chunkY+1,chunkZ)) )
    {
        GLuint p1 = _renderer->AddVertex(v4,0,glm::vec3(0));
        GLuint p2 = _renderer->AddVertex(v7,0,glm::vec3(0));
        GLuint p3 = _renderer->AddVertex(v6,0,glm::vec3(0));

        GLuint p4 = _renderer->AddVertex(v4,0,glm::vec3(0));
        GLuint p5 = _renderer->AddVertex(v5,0,glm::vec3(0));
        GLuint p6 = _renderer->AddVertex(v7,0,glm::vec3(0));

        _renderer->AddTriangle(p1,p2,p3);
        _renderer->AddTriangle(p4,p5,p6);
    }
    //std::cout << "Top" <<std::endl;

    //Bottom
    if(! ((y!=0) ? voxeldata[x][y-1][z] : cm->BlockExistsInChunk(x,CHUNK_SIZE-1,z, chunkX,chunkY-1,chunkZ)))
    {
        GLuint p1 = _renderer->AddVertex(v1,1,glm::vec3(0));
        GLuint p2 = _renderer->AddVertex(v2,1,glm::vec3(0));
        GLuint p3 = _renderer->AddVertex(v3,1,glm::vec3(0));
        GLuint p4 = _renderer->AddVertex(v0,1,glm::vec3(0));

        _renderer->AddTriangle(p1,p2,p3);
        _renderer->AddTriangle(p1,p4,p2);
    }
    //std::cout << "Bottom" <<std::endl;

    //Right
    if(! ((x!=CHUNK_SIZE-1) ? voxeldata[x+1][y][z] : cm->BlockExistsInChunk(0,y,z, chunkX+1,chunkY,chunkZ)))
    {
        GLuint p1 = _renderer->AddVertex(v7,2,glm::vec3(0));
        GLuint p2 = _renderer->AddVertex(v2,2,glm::vec3(0));
        GLuint p3 = _renderer->AddVertex(v6,2,glm::vec3(0));
        GLuint p4 = _renderer->AddVertex(v3,2,glm::vec3(0));

        _renderer->AddTriangle(p1,p2,p3);
        _renderer->AddTriangle(p1,p4,p2);
    }
    //std::cout << "Right" <<std::endl;

    //Left
    if(! ((x!=0) ? voxeldata[x-1][y][z] : cm->BlockExistsInChunk(CHUNK_SIZE-1,y,z, chunkX-1,chunkY,chunkZ)))
    {
        GLuint p1 = _renderer->AddVertex(v4,3,glm::vec3(0));
        GLuint p2 = _renderer->AddVertex(v1,3,glm::vec3(0));
        GLuint p3 = _renderer->AddVertex(v5,3,glm::vec3(0));
        GLuint p4 = _renderer->AddVertex(v0,3,glm::vec3(0));

        _renderer->AddTriangle(p1,p2,p3);
        _renderer->AddTriangle(p1,p4,p2);
    }
    //std::cout << "Left" <<std::endl;

    //Front
    if(! ((z!=CHUNK_SIZE-1) ? voxeldata[x][y][z+1] : cm->BlockExistsInChunk(x,y,0, chunkX,chunkY,chunkZ+1)))
    {
        GLuint p1 = _renderer->AddVertex(v5,4,glm::vec3(0));
        GLuint p2 = _renderer->AddVertex(v3,4,glm::vec3(0));
        GLuint p3 = _renderer->AddVertex(v7,4,glm::vec3(0));
        GLuint p4 = _renderer->AddVertex(v1,4,glm::vec3(0));

        _renderer->AddTriangle(p1,p2,p3);
        _renderer->AddTriangle(p1,p4,p2);
    }
    //std::cout << "Front" <<std::endl;

    //Back
    if(! ((z!=0) ? voxeldata[x][y][z-1] : cm->BlockExistsInChunk(x,y,CHUNK_SIZE-1, chunkX,chunkY,chunkZ-1)))
    {
        GLuint p1 = _renderer->AddVertex(v6,5,glm::vec3(0));
        GLuint p2 = _renderer->AddVertex(v0,5,glm::vec3(0));
        GLuint p3 = _renderer->AddVertex(v4,5,glm::vec3(0));
        GLuint p4 = _renderer->AddVertex(v2,5,glm::vec3(0));

        _renderer->AddTriangle(p1,p2,p3);
        _renderer->AddTriangle(p1,p4,p2);
    }
    //std::cout << "Back" <<std::endl;
}
