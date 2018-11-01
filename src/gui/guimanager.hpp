#ifndef GUIMANAGER_H
#define GUIMANAGER_H

#include <GLFW/glfw3.h>
#include "gui/imgui.h"
#include "gui/imgui_impl_glfw.h"
#include "gui/imgui_impl_opengl3.h"

class GuiManager {
public:
    GuiManager();
    GuiManager(GLFWwindow* window);
    GuiManager(GuiManager& orig);
    virtual ~GuiManager();
    void drawControlPanel(bool* program_exit, ImVec4* clear_color);
    
private:
    bool show_demo_window = false;
};

#endif /* GUIMANAGER_H */


