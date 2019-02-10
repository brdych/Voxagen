#include "terraingenerator.hpp"

TerrainGenerator::TerrainGenerator()
{

}

TerrainGenerator::~TerrainGenerator()
{

}

void TerrainGenerator::GenerateChunk(Chunk* c)
{
    WorldVariables::GEN_LOCK.lock();
    WorldVariables::NUM_GEN_THREADS++;
    WorldVariables::GEN_LOCK.unlock();

    uint CHUNK_SIZE = static_cast<uint>(WorldVariables::CHUNK_SIZE);
    int wxb = (c->chunkX * CHUNK_SIZE);
    int wyb = (c->chunkY * CHUNK_SIZE);
    int wzb = (c->chunkZ * CHUNK_SIZE);
    int index = 0;

    FastNoiseSIMD* p_gen = FastNoiseSIMD::NewFastNoiseSIMD();
    p_gen->SetFrequency(0.0015f);
    p_gen->SetPerturbAmp(2.0f);
    p_gen->SetFractalOctaves(10);
    p_gen->SetPerturbFractalLacunarity(2.0f);
    p_gen->SetFractalGain(0.5f);
    float* p_set = p_gen->GetSimplexFractalSet(wxb, 0, wzb, CHUNK_SIZE, 1, CHUNK_SIZE);

    for (uint x = 0; x < CHUNK_SIZE; x++)
    {
        for (uint z = 0; z < CHUNK_SIZE; z++)
        {
            float val = p_set[index++];
            val = 100+(val*(156));
            int nVal = static_cast<int>(abs(val));

            for (uint y = 0; y < CHUNK_SIZE; y++)
            {
                int wy = wyb+y;
                if(wy - nVal <= 0)
                {
                    c->AddBlock(x,y,z, 3);
                }
                else if(wy - nVal <= 1)
                {
                    if(wy < 66)
                    {
                        c->AddBlock(x,y,z, 4);
                    }
                    else
                    {
                        c->AddBlock(x,y,z, 2);
                    }
                }
                else if(wy <= 64)
                {
                    c->AddBlock(x,y,z, 1);
                }
            }
        }
    }
    FastNoiseSIMD::FreeNoiseSet(p_set);
    delete p_gen;
    c->isGenerated = true;

    WorldVariables::GEN_LOCK.lock();
    WorldVariables::NUM_GEN_THREADS--;
    WorldVariables::GEN_LOCK.unlock();
}
