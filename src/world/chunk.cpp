#include "chunk.hpp"
#include "utility/chunkmanager.hpp"

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

void Chunk::SetMesh(Mesh m)
{
    _mesh = m;
}

Mesh* Chunk::GetMesh()
{
    return &_mesh;
}

bool Chunk::GetBlock(uint x, uint y, uint z)
{
    return voxeldata[x + CHUNK_SIZE * (y + CHUNK_SIZE * z)];
}

void Chunk::AddBlock(uint x, uint y, uint z)
{
    voxeldata[x + CHUNK_SIZE * (y + CHUNK_SIZE * z)] = true;
    _chunkSize++;
}
void Chunk::RemoveBlock(uint x, uint y, uint z)
{
    voxeldata[x + CHUNK_SIZE * (y + CHUNK_SIZE * z)] = false;
    _chunkSize--;
}

bool Chunk::ShouldRender(float fov, glm::vec3* cameraFront, glm::vec3* cameraPos)
{
    if(_chunkSize < 1)
        return false;
    if(_mesh.EmptyMesh())
        return false;

    int x = chunkX*CHUNK_SIZE+(CHUNK_SIZE/2);
    int y = chunkY*CHUNK_SIZE+(CHUNK_SIZE/2);
    int z = chunkZ*CHUNK_SIZE+(CHUNK_SIZE/2);

    float d = glm::dot(glm::normalize(*cameraFront), glm::normalize(glm::vec3(x,y,z) - *cameraPos));
    //std::cout << d << std::endl;
    if(d < 0)
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
