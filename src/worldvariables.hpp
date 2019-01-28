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

    inline static unsigned int CHUNK_SIZE = 64;

    inline static bool PROGRAM_SHOULD_EXIT =    false;
    inline static bool CULLING_ENABLED =        true;
    inline static bool Z_BUFFER_ENABLED =       true;
    inline static bool USE_WIREFRAME =          false;
    inline static bool SHOW_CHUNK_BOUNDS =      false;
    inline static bool MENU =                   false;

    inline static glm::vec4* CLEAR_COLOUR =     new glm::vec4(0.09f, 0.09f, 1.0f, 1.0f);
    inline static glm::vec3* LIGHT_POS =        new glm::vec3(-1.0f,-1.0f,-1.0f);
    inline static glm::vec3* GLOBAL_LIGHT_DIR = new glm::vec3(-1,-2,-3);
    inline static glm::vec3* GLOBAL_LIGHT_COL = new glm::vec3(1,1,1);
    inline static glm::vec3* FOG_INFO =         new glm::vec3(0, 0, 0.001);

    /*static void Init()
    {
        // Floats
        BLOCK_SIZE =            0.5f;
        DELTA_TIME =            0.0f;
        LAST_FRAME =            0.0f;

        // Ints
        CHUNK_SIZE =            64;

        // Vectors
        CLEAR_COLOUR =          new glm::vec4(0.09f, 0.09f, 1.0f, 1.0f);
        LIGHT_POS =             new glm::vec3(-1.0f,-1.0f,-1.0f);
        GLOBAL_LIGHT_COL =      new glm::vec3(1,1,1);
        GLOBAL_LIGHT_DIR =      new glm::vec3(-1,-2,-3);
        FOG_INFO =              new glm::vec3(0, 0, 0.001);

        // Bools
        PROGRAM_SHOULD_EXIT =   false;
        CULLING_ENABLED =       true;
        Z_BUFFER_ENABLED =      true;
        SHOW_CHUNK_BOUNDS =     false;
        ONLY_RENDERING_CHUNKS = false;
        MENU =                  false;
    }*/
};

#endif // WORLDVARIABLES_HPP
