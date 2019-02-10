#include "chunkmesher.hpp"

void GenChunk(Chunk* c, Chunk* top, Chunk* bottom, Chunk* right, Chunk* left, Chunk* front, Chunk* back)
{
    WorldVariables::MESH_LOCK.lock();
    WorldVariables::NUM_MESH_THREADS++;
    WorldVariables::MESH_LOCK.unlock();

    Mesh* m = c->GetMesh(0);
    Mesh* wm = c->GetMesh(1);
    uint CHUNK_SIZE = WorldVariables::CHUNK_SIZE;
    float VOXEL_SIZE = WorldVariables::BLOCK_SIZE;

    m->StartMesh();
    wm->StartMesh();

    for(uint x = 0; x < CHUNK_SIZE; x++)
        for(uint z = 0; z < CHUNK_SIZE; z++)
            for(uint y = 0; y < CHUNK_SIZE; y++)
                if(c->voxeldata[x + CHUNK_SIZE * (y + CHUNK_SIZE * z)]!=0)
                {
                    int ya = y + (c->chunkY*CHUNK_SIZE);

                    unsigned char bt = c->GetBlock(x,y,z);

                    glm::vec3 col;

                    if(bt == 1)
                    {
                      col = glm::vec3(0.0,0.8,1.0);
                    }
                    else if (bt == 2)
                    {
                      col = glm::vec3(0.1,0.4,0.1);
                    }
                    else if (bt == 3)
                    {
                      col = glm::vec3(0.4,0.4,0.4);
                    }
                    else if (bt == 4)
                    {
                      col = glm::vec3(0.8,0.8,0.4);
                    }

                    glm::vec3 v0 = glm::vec3(x,y,z);
                    glm::vec3 v1 = glm::vec3(x,y,z+VOXEL_SIZE);
                    glm::vec3 v2 = glm::vec3(x+VOXEL_SIZE,y,z);
                    glm::vec3 v3 = glm::vec3(x+VOXEL_SIZE,y,z+VOXEL_SIZE);
                    glm::vec3 v4 = glm::vec3(x,y+VOXEL_SIZE,z);
                    glm::vec3 v5 = glm::vec3(x,y+VOXEL_SIZE,z+VOXEL_SIZE);
                    glm::vec3 v6 = glm::vec3(x+VOXEL_SIZE,y+VOXEL_SIZE,z);
                    glm::vec3 v7 = glm::vec3(x+VOXEL_SIZE,y+VOXEL_SIZE,z+VOXEL_SIZE);

                    //std::cout << "Init" <<std::endl;

                    if(bt != 1)
                    {
                        //Top
                        if(! ((y!=CHUNK_SIZE-1) ? c->GetBlock(x, y+1, z) > 1 : top->GetBlock(x, 0, z) > 1))
                        {
                            GLuint p1 = m->AddVertex(v4,0,col);
                            GLuint p2 = m->AddVertex(v7,0,col);
                            GLuint p3 = m->AddVertex(v6,0,col);
                            GLuint p4 = m->AddVertex(v5,0,col);

                            m->AddTriangle(p1,p2,p3);
                            m->AddTriangle(p1,p4,p2);
                        }
                        //std::cout << "Top" <<std::endl;

                        //Bottom
                        if(! ((y!=0) ? c->GetBlock(x, y-1, z) > 1 : bottom->GetBlock(x,CHUNK_SIZE-1,z) > 1))
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
                        if(! ((x!=CHUNK_SIZE-1) ? c->GetBlock(x+1, y, z) > 1 : right->GetBlock(0,y,z) > 1))
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
                        if(! ((x!=0) ? c->GetBlock(x-1, y, z) > 1 : left->GetBlock(CHUNK_SIZE-1,y,z) > 1 ))
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
                        if(! ((z!=CHUNK_SIZE-1) ? c->GetBlock(x, y, z+1) > 1 : front->GetBlock(x,y,0) > 1 ))
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
                        if(! ((z!=0) ? c->GetBlock(x, y, z-1) > 1 : back->GetBlock(x,y,CHUNK_SIZE-1) > 1 ))
                        {
                            GLuint p1 = m->AddVertex(v6,5,col);
                            GLuint p2 = m->AddVertex(v0,5,col);
                            GLuint p3 = m->AddVertex(v4,5,col);
                            GLuint p4 = m->AddVertex(v2,5,col);

                            m->AddTriangle(p1,p2,p3);
                            m->AddTriangle(p1,p4,p2);
                        }
                    }
                    else
                    {
                        //Top
                        if(! ((y!=CHUNK_SIZE-1) ? c->GetBlock(x, y+1, z) > 0 : top->GetBlock(x, 0, z) > 0))
                        {
                            GLuint p1 = wm->AddVertex(v4,0,col);
                            GLuint p2 = wm->AddVertex(v7,0,col);
                            GLuint p3 = wm->AddVertex(v6,0,col);
                            GLuint p4 = wm->AddVertex(v5,0,col);

                            wm->AddTriangle(p1,p2,p3);
                            wm->AddTriangle(p1,p4,p2);
                        }
                        //std::cout << "Top" <<std::endl;

                        //Bottom
                        if(! ((y!=0) ? c->GetBlock(x, y-1, z) > 0 : bottom->GetBlock(x,CHUNK_SIZE-1,z) > 0))
                        {
                            GLuint p1 = wm->AddVertex(v1,1,col);
                            GLuint p2 = wm->AddVertex(v2,1,col);
                            GLuint p3 = wm->AddVertex(v3,1,col);
                            GLuint p4 = wm->AddVertex(v0,1,col);

                            wm->AddTriangle(p1,p2,p3);
                            wm->AddTriangle(p1,p4,p2);
                        }
                        //std::cout << "Bottom" <<std::endl;

                        //Right
                        if(! ((x!=CHUNK_SIZE-1) ? c->GetBlock(x+1, y, z) > 0 : right->GetBlock(0,y,z) > 0))
                        {
                            GLuint p1 = wm->AddVertex(v7,2,col);
                            GLuint p2 = wm->AddVertex(v2,2,col);
                            GLuint p3 = wm->AddVertex(v6,2,col);
                            GLuint p4 = wm->AddVertex(v3,2,col);

                            wm->AddTriangle(p1,p2,p3);
                            wm->AddTriangle(p1,p4,p2);
                        }
                        //std::cout << "Right" <<std::endl;

                        //Left
                        if(! ((x!=0) ? c->GetBlock(x-1, y, z) > 0 : left->GetBlock(CHUNK_SIZE-1,y,z) > 0 ))
                        {
                            GLuint p1 = wm->AddVertex(v4,3,col);
                            GLuint p2 = wm->AddVertex(v1,3,col);
                            GLuint p3 = wm->AddVertex(v5,3,col);
                            GLuint p4 = wm->AddVertex(v0,3,col);

                            wm->AddTriangle(p1,p2,p3);
                            wm->AddTriangle(p1,p4,p2);
                        }
                        //std::cout << "Left" <<std::endl;

                        //Front
                        if(! ((z!=CHUNK_SIZE-1) ? c->GetBlock(x, y, z+1) > 0 : front->GetBlock(x,y,0) > 0 ))
                        {
                            GLuint p1 = wm->AddVertex(v5,4,col);
                            GLuint p2 = wm->AddVertex(v3,4,col);
                            GLuint p3 = wm->AddVertex(v7,4,col);
                            GLuint p4 = wm->AddVertex(v1,4,col);

                            wm->AddTriangle(p1,p2,p3);
                            wm->AddTriangle(p1,p4,p2);
                        }
                        //std::cout << "Front" <<std::endl;

                        //Back
                        if(! ((z!=0) ? c->GetBlock(x, y, z-1) > 0 : back->GetBlock(x,y,CHUNK_SIZE-1) > 0 ))
                        {
                            GLuint p1 = wm->AddVertex(v6,5,col);
                            GLuint p2 = wm->AddVertex(v0,5,col);
                            GLuint p3 = wm->AddVertex(v4,5,col);
                            GLuint p4 = wm->AddVertex(v2,5,col);

                            wm->AddTriangle(p1,p2,p3);
                            wm->AddTriangle(p1,p4,p2);
                        }
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
