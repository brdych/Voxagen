#include "voxagensettings.hpp"

VoxagenSettings* VoxagenSettings::_instance;

VoxagenSettings::VoxagenSettings()
{
    CLEAR_COLOUR =          new glm::vec4(0.09f, 0.09f, 1.0f, 1.0f);
    LIGHT_POS =             new glm::vec3(-1.0f,-1.0f,-1.0f);
    GLOBAL_LIGHT_COL =      new glm::vec3(1,1,1);
    GLOBAL_LIGHT_DIR =      new glm::vec3(-1,-2,-3);
    FOG_INFO =              new glm::vec3(0, 0, 0.003);

    PROGRAM_SHOULD_EXIT =   false;
    CULLING_ENABLED =       true;
    Z_BUFFER_ENABLED =      true;
    SHOW_CHUNK_BOUNDS =     false;
    ONLY_RENDERING_CHUNKS = false;
    MENU =                  false;
}

VoxagenSettings* VoxagenSettings::SettingsInstance()
{
    if(_instance == nullptr)
    {
        _instance = new VoxagenSettings();
    }
    return _instance;
}
