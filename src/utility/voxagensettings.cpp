#include "voxagensettings.hpp"

VoxagenSettings* VoxagenSettings::_instance;

VoxagenSettings::VoxagenSettings()
{
    CLEAR_COLOUR = new glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
    LIGHT_POS = new glm::vec3(-1.0f,-1.0f,-1.0f);

    GLOBAL_LIGHT_COL = new glm::vec3(0.9,0.9,0.9);
    GLOBAL_LIGHT_DIR = new glm::vec3(0,-1,0);

    PROGRAM_SHOULD_EXIT = false;
    CULLING_ENABLED = true;
    Z_BUFFER_ENABLED = true;
}

VoxagenSettings* VoxagenSettings::SettingsInstance()
{
    //std::cout << "Request: VoxagenSettings Instance" << std::endl;
    if(_instance == nullptr)
    {
        std::cout << "- Creating New VoxagenSettings Instance" << std::endl;
        _instance = new VoxagenSettings();
    }
    return _instance;
}
