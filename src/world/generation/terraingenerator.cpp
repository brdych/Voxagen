#include "terraingenerator.hpp"

TerrainGenerator::TerrainGenerator()
{

}

TerrainGenerator::~TerrainGenerator()
{

}

void TerrainGenerator::GenerateChunk(Chunk* c)
{
    //bool vdata[34][34][34];
    int CHUNK_SIZE = WorldVariables::CHUNK_SIZE;
    int wxb = (c->chunkX * CHUNK_SIZE);
    int wyb = (c->chunkY * CHUNK_SIZE);
    int wzb = (c->chunkZ * CHUNK_SIZE);
    int index = 0;

    FastNoiseSIMD* p_gen = FastNoiseSIMD::NewFastNoiseSIMD();
    float* p_set = p_gen->GetSimplexFractalSet(wxb, 0, wzb, CHUNK_SIZE, 1, CHUNK_SIZE);

    for (int x = 0; x < CHUNK_SIZE; x++)
    {
        for (int z = 0; z < CHUNK_SIZE; z++)
        {
            float val = p_set[index++];
            val = 100+(val*(156));
            val = abs(val);





            for (int y = 0; y < CHUNK_SIZE; y++)
            {
                int wy = wyb+y;
                if(wy<100)
                {
                    c->SetBlock(x,y,z,true);
                }
                else
                {
                    c->SetBlock(x,y,z, ((wy<val)?true:false) );
                }
            }
        }
    }
    FastNoiseSIMD::FreeNoiseSet(p_set);
    delete p_gen;
    c->isGenerated = true;
}
