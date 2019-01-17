#ifndef INPUTCONTROLLER_HPP
#define INPUTCONTROLLER_HPP

#include <GLFW/glfw3.h>
#include "camera.hpp"


class InputController
{
public:
    //Functions
    static InputController* GetInputControllerInstance();
    void SetupControls(GLFWwindow* window, Camera* camera);
    void ProcessInput(float deltaTime);
private:
    //Functions
    InputController();
    static void MouseCallback(GLFWwindow* window, double xpos, double ypos);
    static void ScrollCallback(GLFWwindow* window, double xoffset, double yoffset);

    //Variables
    static Camera* camera;
    static GLFWwindow* window;
    static InputController* _Instance;
    static const unsigned int SCR_WIDTH = 1920;
    static const unsigned int SCR_HEIGHT = 1080;
    static float lastX;
    static float lastY;
    static bool firstMouse;
    static bool free_mouse;
    static bool alt_pressed;
};

#endif // INPUTCONTROLLER_HPP
