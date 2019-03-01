#ifndef INPUTCONTROLLER_HPP
#define INPUTCONTROLLER_HPP

#include <GLFW/glfw3.h>
#include "utility/Camera/camera.hpp"
#include "worldvariables.hpp"
#include "glm/common.hpp"
#include "utility/Voxagen/voxagenrenderer.hpp"

class InputController
{
public:
    //Functions
    InputController();
    void ProcessInput(float deltaTime);
private:
    //Functions
    static void MouseCallback(GLFWwindow* window, double xpos, double ypos);
    static void MouseClickCallback(GLFWwindow* window, int button, int action, int mods);
    static void ScrollCallback(GLFWwindow* window, double xoffset, double yoffset);

    //Variables
    static float lastX;
    static float lastY;
    static bool firstMouse;
    static bool free_mouse;
    static bool alt_pressed;
};

#endif // INPUTCONTROLLER_HPP
