#include "weirdgenerator.hpp"

WeirdGenerator::WeirdGenerator()
{

}

WeirdGenerator::~WeirdGenerator()
{

}

void WeirdGenerator::GenerateChunk(Chunk* c)
{
    uint CHUNK_SIZE = static_cast<uint>(WorldVariables::CHUNK_SIZE);
    int wxb = (c->chunkX * CHUNK_SIZE);
    int wyb = (c->chunkY * CHUNK_SIZE);
    int wzb = (c->chunkZ * CHUNK_SIZE);
    int index = 0;

    FastNoiseSIMD* p_gen = FastNoiseSIMD::NewFastNoiseSIMD();
    p_gen->SetFrequency(0.0001f);  // 0.0015f

    p_gen->SetFractalType(FastNoiseSIMD::FractalType::Billow);
    p_gen->SetFractalOctaves(16);
    p_gen->SetFractalGain(0.5f);

    p_gen->SetPerturbType(FastNoiseSIMD::PerturbType::GradientFractal);
    p_gen->SetPerturbFractalLacunarity(0.2f);
    p_gen->SetPerturbAmp(1.0f);

    float* p_set = p_gen->GetSimplexFractalSet(wxb, 0, wzb, CHUNK_SIZE, 1, CHUNK_SIZE);

    for (uint x = 0; x < CHUNK_SIZE; ++x)
    {
        for (uint z = 0; z < CHUNK_SIZE; ++z)
        {
            float val = p_set[index++];
            val = 100+(val*(1000));  // 100 + 156
            int nVal = static_cast<int>(abs(val));

            for (uint y = 0; y < CHUNK_SIZE; ++y)
            {
                int wy = wyb+y;
                if(wy - nVal <= 0 || wy+nVal > 300)
                {
                    c->AddBlock(x,y,z, 3);  // Stone
                }
                else if(wy - nVal <= 1)
                {
                    if(wy < 66)
                    {
                        c->AddBlock(x,y,z, 4);  // Sand
                    }
                    else if(wy < 350)
                    {
                        c->AddBlock(x,y,z, 3);  // Grass
                    }
                    else
                    {
                        c->AddBlock(x,y,z, 5);  // Snow
                    }
                }
                else if(wy <= 63)
                {
                    c->AddBlock(x,y,z, 1);  //Water
                }
            }
        }
    }
    FastNoiseSIMD::FreeNoiseSet(p_set);
    delete p_gen;
    c->isGenerated = true;
}
