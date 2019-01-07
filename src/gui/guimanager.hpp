#ifndef GUIMANAGER_H
#define GUIMANAGER_H

#include <GLFW/glfw3.h>
#include <utility/voxagensettings.hpp>
#include "gui/imgui.h"
#include "gui/imgui_impl_glfw.h"
#include "gui/imgui_impl_opengl3.h"

class GuiManager {
public:
    GuiManager();
    GuiManager(GLFWwindow* window);
    GuiManager(GuiManager& orig);
    virtual ~GuiManager();
    void drawControlPanel();
    
private:
    bool show_demo_window = false;
    VoxagenSettings* _settings = VoxagenSettings::SettingsInstance();
};

#endif /* GUIMANAGER_H */


