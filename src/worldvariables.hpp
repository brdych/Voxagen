#ifndef WORLDVARIABLES_HPP
#define WORLDVARIABLES_HPP

#include <iostream>
#include "glm/vec4.hpp"
#include "glm/vec3.hpp"

class WorldVariables
{
public:

    inline static float BLOCK_SIZE = 0.5f;
    inline static float DELTA_TIME = 0.0f;
    inline static float LAST_FRAME = 0.0f;

    inline static unsigned int CHUNK_SIZE = 32;

    inline static unsigned int CHUNK_LOADLIST_SIZE = 0;
    inline static unsigned int CHUNK_MESHLIST_SIZE = 0;
    inline static unsigned int CHUNK_VISIBLELIST_SIZE = 0;
    inline static unsigned int CHUNK_RENDERLIST_SIZE = 0;
    inline static unsigned int CHUNK_UNLOADLIST_SIZE = 0;
    inline static unsigned int CHUNK_UPDATES_PER_FRAME = 4;

    inline static bool PROGRAM_SHOULD_EXIT =    false;
    inline static bool CULLING_ENABLED =        true;
    inline static bool Z_BUFFER_ENABLED =       true;
    inline static bool USE_WIREFRAME =          false;
    inline static bool SHOW_CHUNK_BOUNDS =      false;
    inline static bool MENU =                   true;
    inline static bool DELETE_ALL =             false;
    inline static bool REBUILD_ALL =            false;



    inline static glm::vec4* CLEAR_COLOUR =     new glm::vec4(0.09f, 0.09f, 1.0f, 1.0f);
    inline static glm::vec3* LIGHT_POS =        new glm::vec3(-1.0f,-1.0f,-1.0f);
    inline static glm::vec3* GLOBAL_LIGHT_DIR = new glm::vec3(-1,-2,-3);
    inline static glm::vec3* GLOBAL_LIGHT_COL = new glm::vec3(1,1,1);
    inline static glm::vec3* FOG_INFO =         new glm::vec3(0, 0, 0.001);
};

#endif // WORLDVARIABLES_HPP
