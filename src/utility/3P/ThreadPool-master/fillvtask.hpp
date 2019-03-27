#ifndef FILLVTASK_HPP
#define FILLVTASK_HPP

#include "vtask.hpp"
#include <vector>
#include <glm/common.hpp>
#include "worldvariables.hpp"
#include "utility/Voxagen/voxagenengine.hpp"
#include "utility/Chunk/chunkhelper.hpp"

/* find maximum of a and b */
#define MAX(a,b) (((a)>(b))?(a):(b))

class FillVTask : public VTask
{
private:
    int x, y, z, a, b, c;
    unsigned char material;
    std::vector<glm::ivec3> points = std::vector<glm::ivec3>();

    // VTask interface
public:
    FillVTask(int x, int y, int z, int a, int b, int c, unsigned char material)
    {
        this->x=x;
        this->y=y;
        this->z=z;
        this->a=a;
        this->b=b;
        this->c=c;
        this->material = material;
    }

    void execute()
    {
        int iDir = (x < a) ? 1 : -1;
        int jDir = (y < b) ? 1 : -1;
        int kDir = (z < c) ? 1 : -1;
        int CHUNK_SIZE = WorldVariables::CHUNK_SIZE;






        for(int i = x; i > a || i < a; i+=iDir)
        {
            for(int j = y; j > b || j < b ; j+=jDir)
            {
                for(int k = z; k > c || k < c; k+=kDir)
                {
                    points.push_back(glm::vec3(i,j,k));
                    std::cout << "ijks: " << i
                                            <<" "<< j
                                            <<" "<< k
                                           << std::endl;
                }
            }
        }

        for(glm::vec3 p: points)
        {
            std::cout << "Point: " << p.x
                                    <<" "<< p.y
                                    <<" "<< p.z
                                   << std::endl;

            glm::vec3 chunkid = glm::vec3(  ChunkHelper::WorldCoordToChunkCoord(p.x),
                                            ChunkHelper::WorldCoordToChunkCoord(p.y),
                                            ChunkHelper::WorldCoordToChunkCoord(p.z));

            auto c = VoxagenEngine::CHUNK_MANAGER.ChunkStore.GetChunkV2(chunkid.x,chunkid.y,chunkid.z);
            bool update = false;
            int x = ChunkHelper::WorldCoordToChunkCoord(p.x);
            int y = ChunkHelper::WorldCoordToChunkCoord(p.y);
            int z = ChunkHelper::WorldCoordToChunkCoord(p.z);
            if(c->GetBlock(x,y,z) == 0)
            {
                c->AddBlock(x,y,z, material);
                update = true;
            }

            if(update)
            {
                c->rebuild = true;
                c->isMeshed = false;
                c->isMeshing = false;
                if(!c->playerData)
                {
                    c->playerData = true;
                }

                if((x < 1 || y <1 || z < 1) || (x >=CHUNK_SIZE-1 || y >= CHUNK_SIZE-1 || z >= CHUNK_SIZE-1))
                {
                    auto a = VoxagenEngine::CHUNK_MANAGER.ChunkStore.GetChunkV2(c->chunkX, c->chunkY+1, c->chunkZ);
                    a->rebuild = true;
                    a->isMeshed = false;
                    a->isMeshing = false;

                    a = VoxagenEngine::CHUNK_MANAGER.ChunkStore.GetChunkV2(c->chunkX, c->chunkY-1, c->chunkZ);
                    a->rebuild = true;
                    a->isMeshed = false;
                    a->isMeshing = false;

                    a = VoxagenEngine::CHUNK_MANAGER.ChunkStore.GetChunkV2(c->chunkX+1, c->chunkY, c->chunkZ);
                    a->rebuild = true;
                    a->isMeshed = false;
                    a->isMeshing = false;

                    a = VoxagenEngine::CHUNK_MANAGER.ChunkStore.GetChunkV2(c->chunkX-1, c->chunkY, c->chunkZ);
                    a->rebuild = true;
                    a->isMeshed = false;
                    a->isMeshing = false;

                    a = VoxagenEngine::CHUNK_MANAGER.ChunkStore.GetChunkV2(c->chunkX, c->chunkY, c->chunkZ+1);
                    a->rebuild = true;
                    a->isMeshed = false;
                    a->isMeshing = false;

                    a = VoxagenEngine::CHUNK_MANAGER.ChunkStore.GetChunkV2(c->chunkX, c->chunkY, c->chunkZ-1);
                    a->rebuild = true;
                    a->isMeshed = false;
                    a->isMeshing = false;
                }
            }
        }

    }
};

#endif // FILLVTASK_HPP
