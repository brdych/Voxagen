#ifndef VOXAGENSETTINGS_HPP
#define VOXAGENSETTINGS_HPP

#include <iostream>
#include "glm/vec4.hpp"
#include "glm/vec3.hpp"



class VoxagenSettings
{
public:
    static VoxagenSettings* SettingsInstance();

    bool PROGRAM_SHOULD_EXIT;
    bool CULLING_ENABLED;
    bool Z_BUFFER_ENABLED;
    bool USE_WIREFRAME;

    glm::vec4* CLEAR_COLOUR;
    glm::vec3* LIGHT_POS;

private:
    VoxagenSettings();
    static VoxagenSettings* _instance;
};

#endif // VOXAGENSETTINGS_H
