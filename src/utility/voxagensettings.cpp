#include "voxagensettings.hpp"

VoxagenSettings* VoxagenSettings::_instance;

VoxagenSettings::VoxagenSettings()
{
    CLEAR_COLOUR = new glm::vec4(0.3f, 0.8f, 1.0f, 1.0f);

    PROGRAM_SHOULD_EXIT = false;
    CULLING_ENABLED = true;
    Z_BUFFER_ENABLED = true;

    std::cout << "Settings Loaded" << std::endl;
}

VoxagenSettings* VoxagenSettings::SettingsInstance()
{
    if(_instance == nullptr)
        _instance = new VoxagenSettings();
    return _instance;
}
