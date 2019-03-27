#include "inputcontroller.hpp"

#include "utility/Chunk/chunk.hpp"
#include "utility/Voxagen/voxagenengine.hpp"
#include "utility/Chunk/chunkhelper.hpp"

float InputController::lastX;
float InputController::lastY;
bool InputController::firstMouse;
bool InputController::free_mouse;
bool InputController::alt_pressed;
bool InputController::key_down;

InputController::InputController()
{
    lastX = VoxagenEngine::SCR_WIDTH / 2.0f;
    lastY = VoxagenEngine::SCR_HEIGHT / 2.0f;
    firstMouse = true;
    free_mouse = false;
    alt_pressed = false;
    key_down = false;

    glfwSetKeyCallback(VoxagenEngine::WINDOW, KeyCallback);
    glfwSetCursorPosCallback(VoxagenEngine::WINDOW,MouseCallback);
    glfwSetScrollCallback(VoxagenEngine::WINDOW, ScrollCallback);
    glfwSetMouseButtonCallback(VoxagenEngine::WINDOW, MouseClickCallback);
    glfwSetInputMode(VoxagenEngine::WINDOW, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}

void InputController::KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if(WorldVariables::EDIT_MODE)
    {
        if (key == GLFW_KEY_Q && action == GLFW_PRESS)
        {
            WorldVariables::EDIT_POS.y--;
        }
        if (key == GLFW_KEY_E && action == GLFW_PRESS)
        {
            WorldVariables::EDIT_POS.y++;
        }

        int dir = WorldVariables::CARDINAL_DIRECTION;
        if (key == GLFW_KEY_W && action == GLFW_PRESS)
        {
            if(dir == 1)      //North Pos X
            {
                WorldVariables::EDIT_POS.x++;
            }
            else if(dir == 2) //East Pos Z
            {
                WorldVariables::EDIT_POS.z++;
            }
            else if(dir == 3) //South Neg X
            {
                WorldVariables::EDIT_POS.x--;
            }
            else                //East Neg Z
            {
                WorldVariables::EDIT_POS.z--;
            }
        }
        if (key == GLFW_KEY_S && action == GLFW_PRESS)
        {
            if(dir == 1)
            {
                WorldVariables::EDIT_POS.x--;
            }
            else if(dir == 2)
            {
                WorldVariables::EDIT_POS.z--;
            }
            else if(dir == 3)
            {
                WorldVariables::EDIT_POS.x++;
            }
            else
            {
                WorldVariables::EDIT_POS.z++;
            }
        }

        if (key == GLFW_KEY_A && action == GLFW_PRESS)
        {
            if(dir == 1)
            {
                WorldVariables::EDIT_POS.z--;
            }
            else if(dir == 2)
            {
                WorldVariables::EDIT_POS.x++;
            }
            else if(dir == 3)
            {
                WorldVariables::EDIT_POS.z++;
            }
            else
            {
                WorldVariables::EDIT_POS.x--;
            }
        }
        if (key == GLFW_KEY_D && action == GLFW_PRESS)
        {
            if(dir == 1)
            {
                WorldVariables::EDIT_POS.z++;
            }
            else if(dir == 2)
            {
                WorldVariables::EDIT_POS.x--;
            }
            else if(dir == 3)
            {
                WorldVariables::EDIT_POS.z--;
            }
            else
            {
                WorldVariables::EDIT_POS.x++;
            }
        }
    }
}

void InputController::MouseCallback(GLFWwindow* window, double xpos, double ypos)
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
        VoxagenEngine::CAMERA.ProcessMouseMovement(xoffset, yoffset);
    }
    else
    {
        firstMouse = true;
    }
}

/* find maximum of a and b */
#define MAX(a,b) (((a)>(b))?(a):(b))

/* absolute value of a */
#define ABS(a) (((a)<0) ? -(a) : (a))

/* take sign of a, either -1, 0, or 1 */
#define ZSGN(a) (((a)<0) ? -1 : (a)>0 ? 1 : 0)

void InputController::MouseClickCallback(GLFWwindow *window, int button, int action, int mods)
{
    if((button == GLFW_MOUSE_BUTTON_RIGHT || button == GLFW_MOUSE_BUTTON_LEFT) && WorldVariables::EDIT_MODE && !free_mouse)
    {
        int CHUNK_SIZE = WorldVariables::CHUNK_SIZE;
        auto c = VoxagenEngine::CHUNK_MANAGER.ChunkStore.GetChunkV2(WorldVariables::EDIT_CHUNK.x, WorldVariables::EDIT_CHUNK.y, WorldVariables::EDIT_CHUNK.z);
        bool update = false;
        int x = WorldVariables::EDIT_CHUNK_POS.x;
        int y = WorldVariables::EDIT_CHUNK_POS.y;
        int z = WorldVariables::EDIT_CHUNK_POS.z;
        if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS)
        {
            if(c->GetBlock(x,y,z) == 0)
            {
                c->AddBlock(x,y,z, WorldVariables::SELECTED_BLOCK);
                update = true;
            }
        }
        else if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
        {
            if(c->GetBlock(x,y,z) != 0)
            {
                c->RemoveBlock(x, y, z);
                update = true;
            }
        }

        if(update)
        {
            c->rebuild = true;
            c->forceRemesh = true;
            c->isMeshed = false;
            c->isMeshing = false;
            if(!c->playerData)
            {
                c->playerData = true;
            }

            if((x < 1 || y <1 || z < 1) || (x >=CHUNK_SIZE-1 || y >= CHUNK_SIZE-1 || z >= CHUNK_SIZE-1))
            {
                auto a = VoxagenEngine::CHUNK_MANAGER.ChunkStore.GetChunkV2(c->chunkX, c->chunkY+1, c->chunkZ);
                a->rebuild = true;
                a->isMeshed = false;
                a->isMeshing = false;

                a = VoxagenEngine::CHUNK_MANAGER.ChunkStore.GetChunkV2(c->chunkX, c->chunkY-1, c->chunkZ);
                a->rebuild = true;
                a->isMeshed = false;
                a->isMeshing = false;

                a = VoxagenEngine::CHUNK_MANAGER.ChunkStore.GetChunkV2(c->chunkX+1, c->chunkY, c->chunkZ);
                a->rebuild = true;
                a->isMeshed = false;
                a->isMeshing = false;

                a = VoxagenEngine::CHUNK_MANAGER.ChunkStore.GetChunkV2(c->chunkX-1, c->chunkY, c->chunkZ);
                a->rebuild = true;
                a->isMeshed = false;
                a->isMeshing = false;

                a = VoxagenEngine::CHUNK_MANAGER.ChunkStore.GetChunkV2(c->chunkX, c->chunkY, c->chunkZ+1);
                a->rebuild = true;
                a->isMeshed = false;
                a->isMeshing = false;

                a = VoxagenEngine::CHUNK_MANAGER.ChunkStore.GetChunkV2(c->chunkX, c->chunkY, c->chunkZ-1);
                a->rebuild = true;
                a->isMeshed = false;
                a->isMeshing = false;
            }
        }
    }
}

void InputController::ScrollCallback(GLFWwindow* window, double xoffset, double yoffset)
{
    VoxagenEngine::CAMERA.ProcessMouseScroll(xoffset, yoffset);
}

void InputController::ProcessInput(float deltaTime)
{
    if (glfwGetKey(VoxagenEngine::WINDOW, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(VoxagenEngine::WINDOW, true);
    if (glfwGetKey(VoxagenEngine::WINDOW, GLFW_KEY_W) == GLFW_PRESS)
        VoxagenEngine::CAMERA.ProcessKeyboard(FORWARD, deltaTime);
    if (glfwGetKey(VoxagenEngine::WINDOW, GLFW_KEY_S) == GLFW_PRESS)
        VoxagenEngine::CAMERA.ProcessKeyboard(BACKWARD, deltaTime);
    if (glfwGetKey(VoxagenEngine::WINDOW, GLFW_KEY_A) == GLFW_PRESS)
        VoxagenEngine::CAMERA.ProcessKeyboard(LEFT, deltaTime);
    if (glfwGetKey(VoxagenEngine::WINDOW, GLFW_KEY_D) == GLFW_PRESS)
        VoxagenEngine::CAMERA.ProcessKeyboard(RIGHT, deltaTime);

    if(glfwGetKey(VoxagenEngine::WINDOW, GLFW_KEY_LEFT_ALT) == GLFW_PRESS)
    {
        if(!free_mouse && !alt_pressed)
        {
            alt_pressed = true;
            free_mouse = true;
            WorldVariables::MENU = true;
            glfwSetInputMode(VoxagenEngine::WINDOW, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        }
        else if (free_mouse && !alt_pressed)
        {
            free_mouse = false;
            alt_pressed = true;
            glfwSetInputMode(VoxagenEngine::WINDOW, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        }
    }
    if(glfwGetKey(VoxagenEngine::WINDOW, GLFW_KEY_LEFT_ALT) == GLFW_RELEASE)
    {
        alt_pressed = false;
    }
    if(glfwGetKey(VoxagenEngine::WINDOW, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
    {
        VoxagenEngine::CAMERA.SpeedToggle = true;
    }
    if(glfwGetKey(VoxagenEngine::WINDOW, GLFW_KEY_LEFT_SHIFT) == GLFW_RELEASE)
    {
        VoxagenEngine::CAMERA.SpeedToggle = false;
    }
}
