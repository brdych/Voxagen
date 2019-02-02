#include "chunkmanager.hpp"
#include "math.h"

ChunkManager* ChunkManager::_instance;
bool ChunkManager::SHUTDOWN = false;

ChunkManager::ChunkManager()
{
    ChunkLoadList = new std::vector<Chunk*>();
    ChunkSetupList = new std::vector<Chunk*>();
    ChunkVisibleList = new std::vector<Chunk*>();
    ChunkRenderList = new std::vector<Chunk*>();
    ChunkUnloadList = new std::vector<Chunk*>();

    _debug = new DebugObject();
    _generator = new ChunkGenerator(new TerrainGenerator());
    _mesher = new ChunkMesher();
    _perlin = new PerlinNoise(1);
}


ChunkManager::~ChunkManager()
{
    delete ChunkLoadList;
    delete ChunkSetupList;
    delete ChunkVisibleList;
    delete ChunkRenderList;
    delete ChunkUnloadList;

    delete _debug;
    delete _generator;
    delete _mesher;
    delete _perlin;
}

void ChunkManager::Update(Camera* camera, GLuint distance)
{
    UpdateLoadList();
    UpdateMeshList();
    UpdateVisibleList(camera);
    UpdateRenderList();
    UpdateUnloadList();
}

void ChunkManager::UpdateUnloadList()
{
    for(uint i = 0; i < ChunkUnloadList->size() && i < WorldVariables::CHUNK_UPDATES_PER_FRAME; i++)
    {
        if(ChunkUnloadList->at(i)->isMeshed)
        {
            Chunk* c = ChunkUnloadList->at(i);
            ChunkUnloadList->at(i) = ChunkUnloadList->back();
            ChunkUnloadList->pop_back();
            delete c;
        }
    }
}

void ChunkManager::UpdateRenderList()
{

}

void ChunkManager::UpdateVisibleList(Camera* c)
{
    int CHUNK_SIZE = WorldVariables::CHUNK_SIZE;
    int distance = 10;
    int px = floor(c->Position.x)/CHUNK_SIZE;
    int py = floor(c->Position.y)/CHUNK_SIZE;
    int pz = floor(c->Position.z)/CHUNK_SIZE;

    for(uint i = 0; i < ChunkVisibleList->size(); i++)
    {
        Chunk* a = ChunkVisibleList->at(i);
        int cx = a->chunkX;
        int cy = a->chunkY;
        int cz = a->chunkZ;
        if(abs(cx-px) > distance || abs(cy-py) > distance || abs(cz-pz) > distance)
        {
            ChunkVisibleList->at(i) = ChunkVisibleList->back();
            ChunkVisibleList->pop_back();
            ChunkUnloadList->push_back(a);
        }
        else if(a->ShouldRender(0, &c->Front, &c->Position))
        {
            ChunkRenderList->push_back(a);
        }
    }
}

void ChunkManager::UpdateMeshList()
{
    uint maxLoads = WorldVariables::CHUNK_UPDATES_PER_FRAME;
    uint curLoads = 0;
    for(uint i = 0; i < ChunkSetupList->size()&&curLoads < maxLoads; i++)
    {
        if(ChunkSetupList->at(i)->isMeshing)
        {
            Chunk* a = ChunkSetupList->at(i);
            ChunkSetupList->at(i) = ChunkSetupList->back();
            ChunkSetupList->pop_back();
            ChunkVisibleList->push_back(a);
        }
        else
        {
            if(ChunkSetupList->at(i)->isGenerated)
            {
                if(ChunkSetupList->at(i)->ShouldMesh())
                {
                    _mesher->GenerateChunkMesh(ChunkSetupList->at(i));
                    ChunkSetupList->at(i)->isMeshing = true;
                    curLoads++;
                }
                else
                {
                    Chunk* a = ChunkSetupList->at(i);
                    ChunkSetupList->at(i) = ChunkSetupList->back();
                    ChunkSetupList->pop_back();
                    ChunkUnloadList->push_back(a);
                }
            }
        }
    }
}

void ChunkManager::UpdateLoadList()
{
    uint maxLoads = WorldVariables::CHUNK_UPDATES_PER_FRAME;
    uint curLoads = 0;
    for(uint i = 0; i < ChunkLoadList->size()&&curLoads < maxLoads; i++)
    {
        if(ChunkLoadList->at(i)->isGenerating)
        {
            Chunk* a = ChunkLoadList->at(i);
            ChunkLoadList->at(i) = ChunkLoadList->back();
            ChunkLoadList->pop_back();
            ChunkSetupList->push_back(a);
        }
        else
        {
            _generator->GenerateChunk(ChunkLoadList->at(i));
            ChunkLoadList->at(i)->isGenerating = true;
            curLoads++;
        }
    }
}


void ChunkManager::RequestChunks()
{
    for(int x = -20; x < 20; x++)
        for (int z = -20; z < 20; z++)
            for(int y = 0; y < 8; y++)
            {
                ChunkLoadList->push_back(new Chunk(x,y,z));
            }
}

ChunkManager* ChunkManager::ChunkManagerInstance()
{
    if(_instance == nullptr)
    {
        _instance = new ChunkManager();
    }
    return _instance;
}

void ChunkManager::Render(glm::mat4* view, glm::mat4* proj, glm::mat4* mvp)
{
    VoxelRenderer::SetupShader();
    for(Chunk*c: *ChunkRenderList)
    {
        if(c->isMeshed)
        {
            c->Render(view, proj, mvp);
        }
    }

    if(WorldVariables::SHOW_CHUNK_BOUNDS)
    {
        for(Chunk*c: *ChunkRenderList)
        {
            _debug->DrawDebugChunk(c->chunkX, c->chunkY, c->chunkZ, *proj, *view);
        }
    }
    ChunkRenderList->clear();
}

bool ChunkManager::BlockExistsInChunk(int x, int y, int z, int cx, int cy, int cz)
{
    int CHUNK_SIZE = WorldVariables::CHUNK_SIZE;
    int wxb = (cx * CHUNK_SIZE);
    int wyb = (cy * CHUNK_SIZE);
    int wzb = (cz * CHUNK_SIZE);
    int wy = wyb+y;

    if(wy<100)
        return true;

    FastNoiseSIMD* p_gen = FastNoiseSIMD::NewFastNoiseSIMD();
    float* p_set = p_gen->GetSimplexFractalSet(wxb+x, 0, wzb+z, 1, 1, 1);

    float val = p_set[0];
    val = 100+(val*(156));
    val = abs(val);

    FastNoiseSIMD::FreeNoiseSet(p_set);
    delete p_gen;
    return ((wy<val)?true:false);
}


bool ChunkManager::GetBlockValue(double x, double y, double z)
{
    if(y<100) return true;  // Water
    float px = x/64;
    float py = z/64;


    float sum = 0;
    float freq = 0.4, amp = 1.0;
    float seed = 0.5;
    float octaves = 10;
    float lacunarity = 1.96;
    float gain = 0.4;

    for (int i=0; i < octaves; i++)
    {
        float n = _perlin->noise(px*freq, py*freq, seed + i / 256.0);
        sum += n*amp;
        freq *= lacunarity;
        amp *= gain;
    }

    //Standard Noise
    sum = sum*150;

    return (y<sum ? true:false);
}
