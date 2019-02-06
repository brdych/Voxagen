#include "chunk.hpp"
#include "utility/chunkmanager.hpp"
#include "glm/glm.hpp"

void Chunk::PrintDebug()
{
    std::string out = "-----------------------------\n";
    out = out + "Chunk: " + std::to_string(chunkX) + " " + std::to_string(chunkY) + " " + std::to_string(chunkZ) + "\n";
    out = out + "-----\n";
    out = out + "IsGenerating?: " + std::to_string(isGenerating) + "\n";
    out = out + "IsGenerated?: " + std::to_string(isGenerated) + "\n";
    out = out + "IsMeshing?: " + std::to_string(isMeshing) + "\n";
    out = out + "IsMeshed?: " + std::to_string(isMeshed) + "\n";
    out = out + "-----\n";
    out = out + "-----------------------------\n";


    std::cout << out << std::endl;
}

Chunk::Chunk(int x, int y, int z) : chunkX(x), chunkY(y), chunkZ(z) {
    voxeldata = std::vector<bool>(CHUNK_SIZE*CHUNK_SIZE*CHUNK_SIZE, false);
    //_renderer = new VoxelRenderer();
    _chunkSize = 0;
    _mesh = Mesh();
}

Chunk::~Chunk()
{
    if(WorldVariables::PROGRAM_SHOULD_EXIT)
    {
        std::cout << "\t\t\tDeleting Chunk: " << chunkX << " " << chunkY << " " << chunkZ << std::endl;
    }
}

void Chunk::SetBlock(uint x, uint y, uint z, bool block)
{
    voxeldata[x + CHUNK_SIZE * (y + CHUNK_SIZE * z)] = block;
    if(block)
    {
        _chunkSize++;
    }
}

bool Chunk::GetBlock(uint x, uint y, uint z)
{
    return voxeldata[x + CHUNK_SIZE * (y + CHUNK_SIZE * z)];
}

bool Chunk::ShouldRender(float fov, glm::vec3* cameraFront, glm::vec3* cameraPos)
{
    if(_chunkSize < 1)
        return false;
    if(!_mesh.ShouldRender())
        return false;
    return true;
}

bool Chunk::ShouldMesh()
{
    return _chunkSize > 0;
}

void Chunk::Render(glm::mat4* view, glm::mat4* proj, glm::mat4* mvp)
{
    if(_chunkSize > 0)
    {
        *mvp = (*proj) * (*view) * glm::translate(glm::mat4(1), glm::vec3(chunkX*Chunk::CHUNK_SIZE,chunkY*Chunk::CHUNK_SIZE,chunkZ*Chunk::CHUNK_SIZE));
        _mesh.Render(*mvp);
    }
}

void Chunk::AddCube(uint x, uint y, uint z, glm::vec3 col)
{
    _mesh.StartMesh();
    //std::cout << "AddCube: " << x << " " << y << " " << z <<std::endl;
    ChunkManager* cm = ChunkManager::ChunkManagerInstance();
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
    if(! ((y!=CHUNK_SIZE-1) ? voxeldata[x + CHUNK_SIZE * ((y+1) + CHUNK_SIZE * z)] : cm->BlockExistsInChunk(x,0,z, chunkX,chunkY+1,chunkZ)) )
    {
        GLuint p1 = _mesh.AddVertex(v4,0,col);
        GLuint p2 = _mesh.AddVertex(v7,0,col);
        GLuint p3 = _mesh.AddVertex(v6,0,col);

        GLuint p4 = _mesh.AddVertex(v4,0,col);
        GLuint p5 = _mesh.AddVertex(v5,0,col);
        GLuint p6 = _mesh.AddVertex(v7,0,col);

        _mesh.AddTriangle(p1,p2,p3);
        _mesh.AddTriangle(p4,p5,p6);
    }
    //std::cout << "Top" <<std::endl;

    //Bottom
    if(! ((y!=0) ? voxeldata[x + CHUNK_SIZE * ((y-1) + CHUNK_SIZE * z)] : cm->BlockExistsInChunk(x,CHUNK_SIZE-1,z, chunkX,chunkY-1,chunkZ)))
    {
        GLuint p1 = _mesh.AddVertex(v1,1,col);
        GLuint p2 = _mesh.AddVertex(v2,1,col);
        GLuint p3 = _mesh.AddVertex(v3,1,col);
        GLuint p4 = _mesh.AddVertex(v0,1,col);

        _mesh.AddTriangle(p1,p2,p3);
        _mesh.AddTriangle(p1,p4,p2);
    }
    //std::cout << "Bottom" <<std::endl;

    //Right
    if(! ((x!=CHUNK_SIZE-1) ? voxeldata[(x+1) + CHUNK_SIZE * (y + CHUNK_SIZE * z)] : cm->BlockExistsInChunk(0,y,z, chunkX+1,chunkY,chunkZ)))
    {
        GLuint p1 = _mesh.AddVertex(v7,2,col);
        GLuint p2 = _mesh.AddVertex(v2,2,col);
        GLuint p3 = _mesh.AddVertex(v6,2,col);
        GLuint p4 = _mesh.AddVertex(v3,2,col);

        _mesh.AddTriangle(p1,p2,p3);
        _mesh.AddTriangle(p1,p4,p2);
    }
    //std::cout << "Right" <<std::endl;

    //Left
    if(! ((x!=0) ? voxeldata[(x-1) + CHUNK_SIZE * (y + CHUNK_SIZE * z)] : cm->BlockExistsInChunk(CHUNK_SIZE-1,y,z, chunkX-1,chunkY,chunkZ)))
    {
        GLuint p1 = _mesh.AddVertex(v4,3,col);
        GLuint p2 = _mesh.AddVertex(v1,3,col);
        GLuint p3 = _mesh.AddVertex(v5,3,col);
        GLuint p4 = _mesh.AddVertex(v0,3,col);

        _mesh.AddTriangle(p1,p2,p3);
        _mesh.AddTriangle(p1,p4,p2);
    }
    //std::cout << "Left" <<std::endl;

    //Front
    if(! ((z!=CHUNK_SIZE-1) ? voxeldata[x + CHUNK_SIZE * (y + CHUNK_SIZE * (z+1))] : cm->BlockExistsInChunk(x,y,0, chunkX,chunkY,chunkZ+1)))
    {
        GLuint p1 = _mesh.AddVertex(v5,4,col);
        GLuint p2 = _mesh.AddVertex(v3,4,col);
        GLuint p3 = _mesh.AddVertex(v7,4,col);
        GLuint p4 = _mesh.AddVertex(v1,4,col);

        _mesh.AddTriangle(p1,p2,p3);
        _mesh.AddTriangle(p1,p4,p2);
    }
    //std::cout << "Front" <<std::endl;

    //Back
    if(! ((z!=0) ? voxeldata[x + CHUNK_SIZE * (y + CHUNK_SIZE * (z-1))] : cm->BlockExistsInChunk(x,y,CHUNK_SIZE-1, chunkX,chunkY,chunkZ-1)))
    {
        GLuint p1 = _mesh.AddVertex(v6,5,col);
        GLuint p2 = _mesh.AddVertex(v0,5,col);
        GLuint p3 = _mesh.AddVertex(v4,5,col);
        GLuint p4 = _mesh.AddVertex(v2,5,col);

        _mesh.AddTriangle(p1,p2,p3);
        _mesh.AddTriangle(p1,p4,p2);
    }
    //std::cout << "Back" <<std::endl;
}
