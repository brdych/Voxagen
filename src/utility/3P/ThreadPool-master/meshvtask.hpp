#ifndef MESHVTASK_HPP
#define MESHVTASK_HPP
#include "vtask.hpp"
#include "utility/Chunk/chunk.hpp"
#include "utility/Generation/worldgenerator.hpp"

class MeshVTask : public VTask
{
private:

    Chunk* c;
    Chunk* top;
    Chunk* bottom;
    Chunk* right;
    Chunk* left;
    Chunk* front;
    Chunk* back;

    // VTask interface
public:
    MeshVTask(Chunk* c, Chunk* top, Chunk* bottom, Chunk* right, Chunk* left, Chunk* front, Chunk* back)
    {
        this->c = c;
        this->top = top;
        this->bottom = bottom;
        this->right = right;
        this->left = left;
        this->front = front;
        this->back = back;
    }
    void execute()
    {
        Mesh* m = c->GetMesh(0);
        Mesh* wm = c->GetMesh(1);
        uint CHUNK_SIZE = WorldVariables::CHUNK_SIZE;
        uint HV = CHUNK_SIZE-1;
        glm::vec3 ao = glm::vec3(WorldVariables::AO_BASE);
        float VOXEL_SIZE = WorldVariables::BLOCK_SIZE;

        m->StartMesh();
        wm->StartMesh();

        for(uint z = 0; z < CHUNK_SIZE; ++z)
            for(uint y = 0; y < CHUNK_SIZE; ++y)
                for(uint x = 0; x < CHUNK_SIZE; ++x)
                    if(c->GetBlock(x,y,z)!=0)
                    {
                        unsigned char blockType = c->GetBlock(x,y,z);
                        unsigned char bTop =    (y!= HV) ? c->GetBlock(x, y+1, z) : top->GetBlock(x, 0, z);
                        unsigned char bBottom = (y!= 0)  ? c->GetBlock(x, y-1, z) : bottom->GetBlock(x, HV, z);
                        unsigned char bRight =  (x!= HV) ? c->GetBlock(x+1, y, z) : right->GetBlock(0, y, z);
                        unsigned char bLeft =   (x!= 0)  ? c->GetBlock(x-1, y, z) : left->GetBlock(HV, y, z);
                        unsigned char bFront =  (z!= HV) ? c->GetBlock(x, y, z+1) : front->GetBlock(x, y, 0);
                        unsigned char bBack =   (z!= 0)  ? c->GetBlock(x, y, z-1) : back->GetBlock(x, y, HV);

                        glm::vec3 col = WorldGenerator::BLOCK_TYPES.at(blockType);
                        glm::vec3 v0 = glm::vec3(x,y,z);
                        glm::vec3 v1 = glm::vec3(x,y,z+VOXEL_SIZE);
                        glm::vec3 v2 = glm::vec3(x+VOXEL_SIZE,y,z);
                        glm::vec3 v3 = glm::vec3(x+VOXEL_SIZE,y,z+VOXEL_SIZE);
                        glm::vec3 v4 = glm::vec3(x,y+VOXEL_SIZE,z);
                        glm::vec3 v5 = glm::vec3(x,y+VOXEL_SIZE,z+VOXEL_SIZE);
                        glm::vec3 v6 = glm::vec3(x+VOXEL_SIZE,y+VOXEL_SIZE,z);
                        glm::vec3 v7 = glm::vec3(x+VOXEL_SIZE,y+VOXEL_SIZE,z+VOXEL_SIZE);

                        if(blockType != 1)
                        {
                            //Top
                            if(!(bTop > 1))
                            {
                                GLuint p1 = m->AddVertex(v4,0,col);
                                GLuint p2 = m->AddVertex(v7,0,col);
                                GLuint p3 = m->AddVertex(v6,0,col);
                                GLuint p4 = m->AddVertex(v5,0,col);
                                m->AddTriangle(p1,p2,p3);
                                m->AddTriangle(p1,p4,p2);
                            }

                            //Bottom
                            if(!(bBottom > 1))
                            {
                                GLuint p1 = m->AddVertex(v1,1,col);
                                GLuint p2 = m->AddVertex(v2,1,col);
                                GLuint p3 = m->AddVertex(v3,1,col);
                                GLuint p4 = m->AddVertex(v0,1,col);
                                m->AddTriangle(p1,p2,p3);
                                m->AddTriangle(p1,p4,p2);
                            }

                            //Right
                            if(!(bRight > 1))
                            {
                                GLuint p1 = m->AddVertex(v7,2,col);
                                GLuint p2 = m->AddVertex(v2,2,col);
                                GLuint p3 = m->AddVertex(v6,2,col);
                                GLuint p4 = m->AddVertex(v3,2,col);
                                m->AddTriangle(p1,p2,p3);
                                m->AddTriangle(p1,p4,p2);
                            }

                            //Left
                            if(!(bLeft > 1))
                            {
                                GLuint p1 = m->AddVertex(v4,3,col);
                                GLuint p2 = m->AddVertex(v1,3,col);
                                GLuint p3 = m->AddVertex(v5,3,col);
                                GLuint p4 = m->AddVertex(v0,3,col);
                                m->AddTriangle(p1,p2,p3);
                                m->AddTriangle(p1,p4,p2);
                            }

                            //Front
                            if(!(bFront > 1))
                            {
                                GLuint p1 = m->AddVertex(v5,4,col);
                                GLuint p2 = m->AddVertex(v3,4,col);
                                GLuint p3 = m->AddVertex(v7,4,col);
                                GLuint p4 = m->AddVertex(v1,4,col);
                                m->AddTriangle(p1,p2,p3);
                                m->AddTriangle(p1,p4,p2);
                            }

                            //Back
                            if(!(bBack > 1))
                            {
                                GLuint p1 = m->AddVertex(v6,5,col);
                                GLuint p2 = m->AddVertex(v0,5,col);
                                GLuint p3 = m->AddVertex(v4,5,col);
                                GLuint p4 = m->AddVertex(v2,5,col);
                                m->AddTriangle(p1,p2,p3);
                                m->AddTriangle(p1,p4,p2);
                            }
                        }
                        else    //Water
                        {
                            //Top
                            if(!(bTop > 0))
                            {
                                GLuint p1 = wm->AddVertex(v4,0,col);
                                GLuint p2 = wm->AddVertex(v7,0,col);
                                GLuint p3 = wm->AddVertex(v6,0,col);
                                GLuint p4 = wm->AddVertex(v5,0,col);
                                wm->AddTriangle(p1,p2,p3);
                                wm->AddTriangle(p1,p4,p2);
                            }

                            //Bottom
                            if(!(bBottom > 0))
                            {
                                GLuint p1 = wm->AddVertex(v1,1,col);
                                GLuint p2 = wm->AddVertex(v2,1,col);
                                GLuint p3 = wm->AddVertex(v3,1,col);
                                GLuint p4 = wm->AddVertex(v0,1,col);
                                wm->AddTriangle(p1,p2,p3);
                                wm->AddTriangle(p1,p4,p2);
                            }

                            //Right
                            if(!(bRight > 0))
                            {
                                GLuint p1 = wm->AddVertex(v7,2,col);
                                GLuint p2 = wm->AddVertex(v2,2,col);
                                GLuint p3 = wm->AddVertex(v6,2,col);
                                GLuint p4 = wm->AddVertex(v3,2,col);
                                wm->AddTriangle(p1,p2,p3);
                                wm->AddTriangle(p1,p4,p2);
                            }

                            //Left
                            if(!(bLeft > 0))
                            {
                                GLuint p1 = wm->AddVertex(v4,3,col);
                                GLuint p2 = wm->AddVertex(v1,3,col);
                                GLuint p3 = wm->AddVertex(v5,3,col);
                                GLuint p4 = wm->AddVertex(v0,3,col);
                                wm->AddTriangle(p1,p2,p3);
                                wm->AddTriangle(p1,p4,p2);
                            }

                            //Front
                            if(!(bFront > 0))
                            {
                                GLuint p1 = wm->AddVertex(v5,4,col);
                                GLuint p2 = wm->AddVertex(v3,4,col);
                                GLuint p3 = wm->AddVertex(v7,4,col);
                                GLuint p4 = wm->AddVertex(v1,4,col);
                                wm->AddTriangle(p1,p2,p3);
                                wm->AddTriangle(p1,p4,p2);
                            }

                            //Back
                            if(!(bBack > 0))
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
        top->DeregisterUse();
        bottom->DeregisterUse();
        right->DeregisterUse();
        left->DeregisterUse();
        front->DeregisterUse();
        back->DeregisterUse();
        c->isMeshed = true;
    }
};

#endif // MESHVTASK_HPP
