#include "inputcontroller.hpp"
#include "chunkmanager.hpp"

InputController* InputController::_Instance;
Camera* InputController::camera;
GLFWwindow* InputController::window;
float InputController::lastX;
float InputController::lastY;
bool InputController::firstMouse;
bool InputController::free_mouse;
bool InputController::alt_pressed;

InputController* InputController::GetInputControllerInstance()
{
    if(_Instance == nullptr)
    {
        _Instance = new InputController();
    }
    return _Instance;
}

InputController::InputController()
{
    lastX = SCR_WIDTH / 2.0f;
    lastY = SCR_HEIGHT / 2.0f;
    firstMouse = true;
    free_mouse = false;
    alt_pressed = false;
}

void InputController::MouseCallback(GLFWwindow* awindow, double xpos, double ypos)
{
    if(!free_mouse)
    {
        if (firstMouse)
        {
            lastX = xpos;
            lastY = ypos;
            firstMouse = false;
        }
        float xoffset = xpos - lastX;
        float yoffset = lastY - ypos;
        lastX = xpos;
        lastY = ypos;
        camera->ProcessMouseMovement(xoffset, yoffset);
    }
    else
    {
        firstMouse = true;
    }
}

void InputController::MouseClickCallback(GLFWwindow *window, int button, int action, int mods)
{
    int CHUNK_SIZE = WorldVariables::CHUNK_SIZE;
    glm::vec3 cp = WorldVariables::CUR_CHUNK;
    glm::vec3 cb = WorldVariables::CUR_POS_CHUNK;
    if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS)
    {
        Chunk* c = ChunkManager::ChunkManagerInstance()->ChunkStore->GetChunk(cp.x,cp.y,cp.z);
        if(c->isMeshed)
        {
            c->AddBlock(cb.x,cb.y,cb.z);
            c->isMeshed = false;
            c->isMeshing = false;
        }
    }
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
    {
        Chunk* c = ChunkManager::ChunkManagerInstance()->ChunkStore->GetChunk(cp.x,cp.y,cp.z);
        if(c->isMeshed)
        {
            c->RemoveBlock(cb.x,cb.y,cb.z);
            c->isMeshed = false;
            c->isMeshing = false;
        }
    }
}

void InputController::ScrollCallback(GLFWwindow* window, double xoffset, double yoffset)
{
    camera->ProcessMouseScroll(xoffset);
}

void InputController::SetupControls(GLFWwindow* window, Camera* camera)
{
    this->camera = camera;
    this->window = window;
    glfwSetCursorPosCallback(window,MouseCallback);
    glfwSetScrollCallback(window, ScrollCallback);
    glfwSetMouseButtonCallback(window, MouseClickCallback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}

void InputController::ProcessInput(float deltaTime)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera->ProcessKeyboard(FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera->ProcessKeyboard(BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera->ProcessKeyboard(LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera->ProcessKeyboard(RIGHT, deltaTime);

    if(glfwGetKey(window, GLFW_KEY_LEFT_ALT) == GLFW_PRESS)
    {
        if(!free_mouse && !alt_pressed)
        {
            alt_pressed = true;
            free_mouse = true;
            WorldVariables::MENU = true;
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        }
        else if (free_mouse && !alt_pressed)
        {
            free_mouse = false;
            alt_pressed = true;
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        }
    }
    if(glfwGetKey(window, GLFW_KEY_LEFT_ALT) == GLFW_RELEASE)
    {
        alt_pressed = false;
    }
    if(glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
    {
        camera->SpeedToggle = true;
    }
    if(glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_RELEASE)
    {
        camera->SpeedToggle = false;
    }
}
