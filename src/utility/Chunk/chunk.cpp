#include "chunk.hpp"
#include "chunkhelper.hpp"

void Chunk::RegisterUse()
{
    _userLock.lock();
    _numUsers++;
    _userLock.unlock();
}

void Chunk::DeregisterUse()
{
    _userLock.lock();
    _numUsers--;
    _userLock.unlock();
}

uint Chunk::UseStatus()
{
    return _numUsers;
}

void Chunk::PrintDebug()
{
    std::string out = "-----------------------------\n";
    out = out + "Chunk: " + std::to_string(chunkX) + " " + std::to_string(chunkY) + " " + std::to_string(chunkZ) + "\n";
    out = out + "-----\n";
    out = out + "IsGenerating?: " + std::to_string(isGenerating) + "\n";
    out = out + "IsGenerated?: " + std::to_string(isGenerated) + "\n";
    out = out + "IsMeshing?: " + std::to_string(isMeshing) + "\n";
    out = out + "IsMeshed?: " + std::to_string(isMeshed) + "\n";
    out = out + "IsRebuild?: " + std::to_string(rebuild) + "\n";
    out = out + "IsInit?: " + std::to_string(isInitialised) + "\n";
    out = out + "ChunkSize: " + std::to_string(_chunkSize) + "\n";
    out = out + "NumUsers: " + std::to_string(_numUsers) + "\n";
    out = out + "-----\n";
    out = out + "-----------------------------\n";
    std::cout << out << std::endl;
}

void Chunk::Init()
{
    //voxeldata = std::vector<unsigned char>(CHUNK_SIZE * CHUNK_SIZE * CHUNK_SIZE, 0);
    isInitialised = true;
}

void Chunk::DeInit()
{
    //voxeldata.clear();
    isInitialised = false;
}

Chunk::Chunk(int x, int y, int z) : chunkX(x), chunkY(y), chunkZ(z) {
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

void Chunk::SetMesh(Mesh m, int i)
{

    if(i == 0)
        _mesh = m;
    else if(i == 1)
        _wmesh = m;
}

Mesh* Chunk::GetMesh(int m)
{
    if(m == 0)
        return &_mesh;
    else if(m == 1)
        return &_wmesh;
}

unsigned char Chunk::GetBlock(uint x, uint y, uint z)
{
    if(x < CHUNK_SIZE && y < CHUNK_SIZE && z < CHUNK_SIZE)
    {
        return voxeldata.at(x,y,z);
        //return voxeldata[x + CHUNK_SIZE * (y + CHUNK_SIZE * z)];
    }
    else {
        std::cout << x << " " << y << " " << z << std::endl;
    }
    return 0;
}

void Chunk::AddBlock(uint x, uint y, uint z, unsigned char blockType)
{
    if(x < CHUNK_SIZE && y < CHUNK_SIZE && z < CHUNK_SIZE)
    {
        //voxeldata[x + CHUNK_SIZE * (y + CHUNK_SIZE * z)] = blockType;
        voxeldata.set(x,y,z, blockType);
        _chunkSize++;
    }
    else {
        std::cout << x << " " << y << " " << z << std::endl;
    }
}
void Chunk::RemoveBlock(uint x, uint y, uint z)
{
    if(x < CHUNK_SIZE && y < CHUNK_SIZE && z < CHUNK_SIZE)
    {
        //voxeldata[x + CHUNK_SIZE * (y + CHUNK_SIZE * z)] = 0;
        voxeldata.set(x,y,z,0);
        _chunkSize--;
    }
    else {
        std::cout << x << " " << y << " " << z << std::endl;
    }
}

bool Chunk::ShouldRender(float fov, glm::vec3* cameraFront, glm::vec3* cameraPos)
{
    if(_chunkSize < 1)
        return false;
    if(_mesh.EmptyMesh() && _wmesh.EmptyMesh())
        return false;

    if(ChunkHelper::DistanceBetween(chunkX, WorldVariables::CUR_CHUNK.x) <= 1 &&
       ChunkHelper::DistanceBetween(chunkY, WorldVariables::CUR_CHUNK.y)  <= 1&&
       ChunkHelper::DistanceBetween(chunkZ, WorldVariables::CUR_CHUNK.z)  <= 1)
    {
      return true;
    }

    else if(!WorldVariables::EDIT_MODE)
    {
        int x = chunkX*CHUNK_SIZE+(CHUNK_SIZE/2);
        int y = chunkY*CHUNK_SIZE+(CHUNK_SIZE/2);
        int z = chunkZ*CHUNK_SIZE+(CHUNK_SIZE/2);
        if(glm::dot(glm::normalize(*cameraFront), glm::normalize(glm::vec3(x,y,z) - *cameraPos)) < WorldVariables::CULL_VALUE)
            return false;
    }
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
        *mvp = (*proj) * (*view) * glm::translate(glm::mat4(1), glm::vec3(chunkX*CHUNK_SIZE,chunkY*CHUNK_SIZE,chunkZ*CHUNK_SIZE));
        _mesh.Render(*mvp);
    }
}

void Chunk::RenderWater(glm::mat4* view, glm::mat4* proj, glm::mat4* mvp)
{
  if(_chunkSize > 0)
  {
      *mvp = (*proj) * (*view) * glm::translate(glm::mat4(1), glm::vec3(chunkX*CHUNK_SIZE,chunkY*CHUNK_SIZE,chunkZ*CHUNK_SIZE));
      _wmesh.Render(*mvp);
  }
}
