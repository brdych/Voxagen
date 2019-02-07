#include "weirdgenerator.hpp"

WeirdGenerator::WeirdGenerator()
{

}

WeirdGenerator::~WeirdGenerator()
{

}

void WeirdGenerator::GenerateChunk(Chunk* c)
{
    WorldVariables::GEN_LOCK.lock();
    WorldVariables::NUM_GEN_THREADS++;
    WorldVariables::GEN_LOCK.unlock();

    int CHUNK_SIZE = WorldVariables::CHUNK_SIZE;
    int wxb = (c->chunkX * CHUNK_SIZE);
    int wyb = (c->chunkY * CHUNK_SIZE);
    int wzb = (c->chunkZ * CHUNK_SIZE);
    int index = 0;

    FastNoiseSIMD* p_gen = FastNoiseSIMD::NewFastNoiseSIMD();
    //p_gen->SetFrequency(1);
    //p_gen->SetPerturbAmp(1.9);
    //p_gen->SetFractalOctaves(1.5);
    //p_gen->SetPerturbFractalLacunarity(1.2);
    //p_gen->SetFractalGain(0.4);
    //float* p_set = p_gen->GetSimplexFractalSet(wxb, 0, wzb, CHUNK_SIZE, 1, CHUNK_SIZE);
    float* p_set = p_gen->GetSimplexSet(wxb, wyb, wzb, CHUNK_SIZE, CHUNK_SIZE, CHUNK_SIZE);
    //p_gen->get

    for (int x = 0; x < CHUNK_SIZE; x++)
    {
        for (int z = 0; z < CHUNK_SIZE; z++)
        {
            for (int y = 0; y < CHUNK_SIZE; y++)
            {
                float val = p_set[index++];
                if(val < -0.2 || val > 0.2)
                {
                    c->AddBlock(x,y,z);
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
