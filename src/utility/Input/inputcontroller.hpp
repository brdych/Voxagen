#ifndef INPUTCONTROLLER_HPP
#define INPUTCONTROLLER_HPP

#include <GLFW/glfw3.h>
#include "utility/Camera/camera.hpp"
#include "worldvariables.hpp"
#include "glm/common.hpp"

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
    static void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
    //Variables
    static float lastX;
    static float lastY;
    static bool firstMouse;
    static bool free_mouse;
    static bool alt_pressed;
    static bool key_down;
};

#endif // INPUTCONTROLLER_HPP
