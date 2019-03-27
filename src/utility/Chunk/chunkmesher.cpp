#include "chunkmesher.hpp"
#include "utility/Generation/worldgenerator.hpp"
#include "utility/Voxagen/voxagenengine.hpp"
#include "utility/3P/ThreadPool-master/meshvtask.hpp"

ChunkMesher::ChunkMesher()
{

}

void ChunkMesher::GenerateChunkMesh(Chunk* c, Chunk* top, Chunk* bottom, Chunk* right, Chunk* left, Chunk* front, Chunk* back)
{
    VoxagenEngine::POOL->enqueue(new MeshVTask(c,top,bottom,right,left,front,back));
}
