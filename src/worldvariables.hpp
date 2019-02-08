#ifndef WORLDVARIABLES_HPP
#define WORLDVARIABLES_HPP

#include <iostream>
#include <mutex>
#include "glm/vec4.hpp"
#include "glm/vec3.hpp"

class WorldVariables
{
public:

    inline static float BLOCK_SIZE =    0.5f;
    inline static float DELTA_TIME =    0.0f;
    inline static float LAST_FRAME =    0.0f;
    inline static float AO_BASE =       0.1f;

    inline static float LOADCHUNKS_TIME = 0.0f;
    inline static float LOADLIST_TIME = 0.0f;
    inline static float MESHLIST_TIME = 0.0f;
    inline static float VISIBLELIST_TIME = 0.0f;
    inline static float RENDERLIST_TIME = 0.0f;
    inline static float EMPTYLIST_TIME = 0.0f;
    inline static float UNLOADLIST_TIME = 0.0f;

    inline static int VIEW_DISTANCE = 4;

    inline static unsigned int CHUNK_SIZE = 64;
    inline static unsigned int CHUNK_UPDATES_PER_FRAME = 8;

    inline static std::mutex GEN_LOCK = std::mutex();
    inline static unsigned int NUM_GEN_THREADS = 0;
    inline static std::mutex MESH_LOCK = std::mutex();
    inline static unsigned int NUM_MESH_THREADS = 0;

    inline static bool PROGRAM_SHOULD_EXIT =    false;
    inline static bool CULLING_ENABLED =        true;
    inline static bool Z_BUFFER_ENABLED =       true;
    inline static bool USE_WIREFRAME =          false;
    inline static bool MENU =                   true;
    inline static bool DELETE_ALL =             false;
    inline static bool REBUILD_ALL =            false;
    inline static bool FREEZE_RENDERLIST=       false;

    inline static bool SHOW_LOAD_BOUNDS =       false;
    inline static bool SHOW_MESH_BOUNDS =       false;
    inline static bool SHOW_VISIBLE_BOUNDS =    false;
    inline static bool SHOW_RENDER_BOUNDS =     false;
    inline static bool SHOW_UNLOAD_BOUNDS =     false;
    inline static bool SHOW_EMPTY_BOUNDS =      false;

    inline static glm::vec3 CLEAR_COLOUR =     glm::vec3(0.09f, 0.09f, 1.0f);
    inline static glm::vec3 LIGHT_POS =        glm::vec3(-1.0f,-1.0f,-1.0f);
    inline static glm::vec3 GLOBAL_LIGHT_DIR = glm::vec3(-1,-2,-3);
    inline static glm::vec3 GLOBAL_LIGHT_COL = glm::vec3(1,1,1);
    inline static glm::vec3 FOG_INFO =         glm::vec3(0, 0, 0.001);

    inline static glm::vec3 CUR_POS;
    inline static glm::vec3 CUR_POS_INT;
    inline static glm::vec3 CUR_CHUNK;
    inline static glm::vec3 CUR_POS_CHUNK;
};

#endif // WORLDVARIABLES_HPP
