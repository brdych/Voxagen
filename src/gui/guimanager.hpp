#ifndef GUIMANAGER_H
#define GUIMANAGER_H

#include <GLFW/glfw3.h>

#include "gui/imgui.h"
#include "gui/imgui_impl_glfw.h"
#include "gui/imgui_impl_opengl3.h"
#include "utility/Camera/camera.hpp"
#include "worldvariables.hpp"

class GuiManager {
public:
    GuiManager();
    GuiManager(GLFWwindow* window);
    virtual ~GuiManager();
    void drawControlPanel();
private:
    bool show_demo_window = false;
    bool show_chunk_panel = false;
    bool show_camera_panel = false;
    bool show_chunkio_panel = false;
    bool show_edit_panel = false;
    bool show_gl_panel = false;
    bool show_world_panel = false;

    int  view_distance = 10;

    void drawChunkPanel(bool* p_open);
    void drawCameraPanel(bool* p_open);
    void drawChunkIOPanel(bool* p_open);
    void drawEditPanel(bool* p_open);
    void drawGLPanel(bool* p_open);
    void drawWorldPanel(bool* p_open);
};

#endif /* GUIMANAGER_H */


