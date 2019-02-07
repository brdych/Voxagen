#include "chunkmesher.hpp"

void GenChunk(Chunk* c, Chunk* top, Chunk* bottom, Chunk* right, Chunk* left, Chunk* front, Chunk* back)
{
    WorldVariables::MESH_LOCK.lock();
    WorldVariables::NUM_MESH_THREADS++;
    WorldVariables::MESH_LOCK.unlock();

    Mesh* m = c->GetMesh();
    uint CHUNK_SIZE = WorldVariables::CHUNK_SIZE;
    float VOXEL_SIZE = WorldVariables::BLOCK_SIZE;

    m->StartMesh();
    for(uint x = 0; x < CHUNK_SIZE; x++)
        for(uint z = 0; z < CHUNK_SIZE; z++)
            for(uint y = 0; y < CHUNK_SIZE; y++)
                if(c->voxeldata[x + CHUNK_SIZE * (y + CHUNK_SIZE * z)])
                {
                    int ya = y + (c->chunkY*CHUNK_SIZE);
                    glm::vec3 col;
                    if(ya < 100)        {col = glm::vec3(0.0,0.8,1.0);}
                    else if(ya < 130)   {col = glm::vec3(0.8,0.8,0.4);}
                    else if (ya < 200)  {col = glm::vec3(0.1,0.4,0.1);}
                    else if (ya < 230)  {col = glm::vec3(0.5,0.4,0.3);}
                    else                {col = glm::vec3(1,1,1);}

                    glm::vec3 v0 = glm::vec3(x-VOXEL_SIZE,y-VOXEL_SIZE,z-VOXEL_SIZE);
                    glm::vec3 v1 = glm::vec3(x-VOXEL_SIZE,y-VOXEL_SIZE,z+VOXEL_SIZE);
                    glm::vec3 v2 = glm::vec3(x+VOXEL_SIZE,y-VOXEL_SIZE,z-VOXEL_SIZE);
                    glm::vec3 v3 = glm::vec3(x+VOXEL_SIZE,y-VOXEL_SIZE,z+VOXEL_SIZE);
                    glm::vec3 v4 = glm::vec3(x-VOXEL_SIZE,y+VOXEL_SIZE,z-VOXEL_SIZE);
                    glm::vec3 v5 = glm::vec3(x-VOXEL_SIZE,y+VOXEL_SIZE,z+VOXEL_SIZE);
                    glm::vec3 v6 = glm::vec3(x+VOXEL_SIZE,y+VOXEL_SIZE,z-VOXEL_SIZE);
                    glm::vec3 v7 = glm::vec3(x+VOXEL_SIZE,y+VOXEL_SIZE,z+VOXEL_SIZE);

                    //std::cout << "Init" <<std::endl;

                    //Top
                    if(! ((y!=CHUNK_SIZE-1) ? c->GetBlock(x, y+1, z) : top->GetBlock(x, 0, z) ))
                    {
                        GLuint p1 = m->AddVertex(v4,0,col);
                        GLuint p2 = m->AddVertex(v7,0,col);
                        GLuint p3 = m->AddVertex(v6,0,col);

                        GLuint p4 = m->AddVertex(v4,0,col);
                        GLuint p5 = m->AddVertex(v5,0,col);
                        GLuint p6 = m->AddVertex(v7,0,col);

                        m->AddTriangle(p1,p2,p3);
                        m->AddTriangle(p4,p5,p6);
                    }
                    //std::cout << "Top" <<std::endl;

                    //Bottom
                    if(! ((y!=0) ? c->voxeldata[x + CHUNK_SIZE * ((y-1) + CHUNK_SIZE * z)] : bottom->GetBlock(x,CHUNK_SIZE-1,z)))
                    {
                        GLuint p1 = m->AddVertex(v1,1,col);
                        GLuint p2 = m->AddVertex(v2,1,col);
                        GLuint p3 = m->AddVertex(v3,1,col);
                        GLuint p4 = m->AddVertex(v0,1,col);

                        m->AddTriangle(p1,p2,p3);
                        m->AddTriangle(p1,p4,p2);
                    }
                    //std::cout << "Bottom" <<std::endl;

                    //Right
                    if(! ((x!=CHUNK_SIZE-1) ? c->voxeldata[(x+1) + CHUNK_SIZE * (y + CHUNK_SIZE * z)] : right->GetBlock(0,y,z)))
                    {
                        GLuint p1 = m->AddVertex(v7,2,col);
                        GLuint p2 = m->AddVertex(v2,2,col);
                        GLuint p3 = m->AddVertex(v6,2,col);
                        GLuint p4 = m->AddVertex(v3,2,col);

                        m->AddTriangle(p1,p2,p3);
                        m->AddTriangle(p1,p4,p2);
                    }
                    //std::cout << "Right" <<std::endl;

                    //Left
                    if(! ((x!=0) ? c->voxeldata[(x-1) + CHUNK_SIZE * (y + CHUNK_SIZE * z)] : left->GetBlock(CHUNK_SIZE-1,y,z)))
                    {
                        GLuint p1 = m->AddVertex(v4,3,col);
                        GLuint p2 = m->AddVertex(v1,3,col);
                        GLuint p3 = m->AddVertex(v5,3,col);
                        GLuint p4 = m->AddVertex(v0,3,col);

                        m->AddTriangle(p1,p2,p3);
                        m->AddTriangle(p1,p4,p2);
                    }
                    //std::cout << "Left" <<std::endl;

                    //Front
                    if(! ((z!=CHUNK_SIZE-1) ? c->voxeldata[x + CHUNK_SIZE * (y + CHUNK_SIZE * (z+1))] : front->GetBlock(x,y,0)))
                    {
                        GLuint p1 = m->AddVertex(v5,4,col);
                        GLuint p2 = m->AddVertex(v3,4,col);
                        GLuint p3 = m->AddVertex(v7,4,col);
                        GLuint p4 = m->AddVertex(v1,4,col);

                        m->AddTriangle(p1,p2,p3);
                        m->AddTriangle(p1,p4,p2);
                    }
                    //std::cout << "Front" <<std::endl;

                    //Back
                    if(! ((z!=0) ? c->voxeldata[x + CHUNK_SIZE * (y + CHUNK_SIZE * (z-1))] : back->GetBlock(x,y,CHUNK_SIZE-1)))
                    {
                        GLuint p1 = m->AddVertex(v6,5,col);
                        GLuint p2 = m->AddVertex(v0,5,col);
                        GLuint p3 = m->AddVertex(v4,5,col);
                        GLuint p4 = m->AddVertex(v2,5,col);

                        m->AddTriangle(p1,p2,p3);
                        m->AddTriangle(p1,p4,p2);
                    }
                }

    c->isMeshed = true;
    top->inUse = false;
    bottom->inUse = false;
    right->inUse = false;
    left->inUse = false;
    front->inUse = false;
    back->inUse = false;

    WorldVariables::MESH_LOCK.lock();
    WorldVariables::NUM_MESH_THREADS--;
    WorldVariables::MESH_LOCK.unlock();
}

ChunkMesher::ChunkMesher()
{

}

void ChunkMesher::GenerateChunkMesh(Chunk* c, Chunk* top, Chunk* bottom, Chunk* right, Chunk* left, Chunk* front, Chunk* back)
{
    std::thread t(GenChunk,c, top, bottom, right, left, front, back);
    t.detach();
}
