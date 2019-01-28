#ifndef GUIMANAGER_H
#define GUIMANAGER_H

#include <GLFW/glfw3.h>

#include "gui/imgui.h"
#include "gui/imgui_impl_glfw.h"
#include "gui/imgui_impl_opengl3.h"
#include "utility/camera.hpp"
#include "worldvariables.hpp"

class GuiManager {
public:
    GuiManager();
    GuiManager(GLFWwindow* window);
    virtual ~GuiManager();
    void drawControlPanel(Camera* c);
private:
    bool show_demo_window = false;
};

#endif /* GUIMANAGER_H */


