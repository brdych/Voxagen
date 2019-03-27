#include "terraingenerator.hpp"
#include "utility/Voxagen/voxagenengine.hpp"

TerrainGenerator::TerrainGenerator()
{
    BLOCK_TYPES.emplace(1, glm::vec3(0.0,0.6,1.0));
    BLOCK_TYPES.emplace(2, glm::vec3(0.1,0.4,0.1));
    BLOCK_TYPES.emplace(3, glm::vec3(0.4,0.4,0.4));
    BLOCK_TYPES.emplace(4, glm::vec3(0.8,0.8,0.4));
    BLOCK_TYPES.emplace(5, glm::vec3(0.8,0.8,0.9));
    BLOCK_TYPES.emplace(6, glm::vec3(0.8,0.7,0.5));


    BLOCK_TYPES.emplace(7, glm::vec3(0.7,0.7,0.7)); // Stone
    BLOCK_TYPES.emplace(8, glm::vec3(0.3,0.2,0.0)); // Wood
    BLOCK_TYPES.emplace(9, glm::vec3(0.6,0.1,0.0)); // Brick
}

TerrainGenerator::~TerrainGenerator()
{
    BLOCK_TYPES.clear();
}

void TerrainGenerator::GenerateChunk(Chunk* c)
{
    uint CHUNK_SIZE = static_cast<uint>(WorldVariables::CHUNK_SIZE);
    c->Init();
    int wxb = (c->chunkX * CHUNK_SIZE);
    int wyb = (c->chunkY * CHUNK_SIZE);
    int wzb = (c->chunkZ * CHUNK_SIZE);
    int index = 0;

    if(wyb > -100)
    {
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
                    if(wy - nVal <= 0)
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
                            c->AddBlock(x,y,z, 2);  // Grass
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
    }
    else if(wyb < 0 && wyb > -600)
    {
        FastNoiseSIMD* p_gen = FastNoiseSIMD::NewFastNoiseSIMD();
        p_gen->SetSeed(8484);
        p_gen->SetFrequency(0.0002f);  // 0.0015f

        p_gen->SetFractalType(FastNoiseSIMD::FractalType::Billow);
        p_gen->SetFractalOctaves(16);
        p_gen->SetFractalGain(0.6f);

        p_gen->SetPerturbType(FastNoiseSIMD::PerturbType::GradientFractal);
        p_gen->SetPerturbFractalLacunarity(0.5f);
        p_gen->SetPerturbAmp(2.0f);

        float* p_set = p_gen->GetSimplexFractalSet(wxb, 0, wzb, CHUNK_SIZE, 1, CHUNK_SIZE);

        for (uint x = 0; x < CHUNK_SIZE; ++x)
        {
            for (uint z = 0; z < CHUNK_SIZE; ++z)
            {
                float val = p_set[index++];
                val = 10+(val*(800));  // 100 + 156
                int nVal = static_cast<int>(abs(val));

                for (uint y = 0; y < CHUNK_SIZE; ++y)
                {
                    int wy = wyb+y;
                    if(wy + nVal >= -64)
                    {
                        c->AddBlock(x,y,z, 3);  // Stone
                    }
                }
            }
        }
        FastNoiseSIMD::FreeNoiseSet(p_set);
        delete p_gen;
    }
    else if(wyb <= -600 && wyb)
    {
        FastNoiseSIMD* p_gen = FastNoiseSIMD::NewFastNoiseSIMD();
        p_gen->SetSeed(8484);
        p_gen->SetFrequency(0.0002f);  // 0.0015f

        p_gen->SetFractalType(FastNoiseSIMD::FractalType::Billow);
        p_gen->SetFractalOctaves(16);
        p_gen->SetFractalGain(0.6f);

        p_gen->SetPerturbType(FastNoiseSIMD::PerturbType::GradientFractal);
        p_gen->SetPerturbFractalLacunarity(0.5f);
        p_gen->SetPerturbAmp(2.0f);

        float* p_set = p_gen->GetSimplexFractalSet(wxb, 0, wzb, CHUNK_SIZE, 1, CHUNK_SIZE);

        for (uint x = 0; x < CHUNK_SIZE; ++x)
        {
            for (uint z = 0; z < CHUNK_SIZE; ++z)
            {
                float val = p_set[index++];
                val = 10+(val*(800));  // 100 + 156
                int nVal = static_cast<int>(abs(val));

                for (uint y = 0; y < CHUNK_SIZE; ++y)
                {
                    int wy = wyb+y;
                    if(wy - nVal <= -1064)
                    {
                        c->AddBlock(x,y,z, 3);  // Stone
                    }
                    else if(wy <= -630)
                    {
                        c->AddBlock(x,y,z, 1);  //Water
                    }
                }
            }
        }
        FastNoiseSIMD::FreeNoiseSet(p_set);
        delete p_gen;
    }


    //VoxagenEngine::FILE_IO->SaveChunk(c);
    c->isGenerated = true;
}
