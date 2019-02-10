#include "inputcontroller.hpp"

#include "world/chunk.hpp"
#include "voxagenengine.hpp"

float InputController::lastX;
float InputController::lastY;
bool InputController::firstMouse;
bool InputController::free_mouse;
bool InputController::alt_pressed;

InputController::InputController()
{
    lastX = VoxagenRenderer::SCR_WIDTH / 2.0f;
    lastY = VoxagenRenderer::SCR_HEIGHT / 2.0f;
    firstMouse = true;
    free_mouse = false;
    alt_pressed = false;

    glfwSetCursorPosCallback(VoxagenRenderer::WINDOW,MouseCallback);
    glfwSetScrollCallback(VoxagenRenderer::WINDOW, ScrollCallback);
    glfwSetMouseButtonCallback(VoxagenRenderer::WINDOW, MouseClickCallback);
    glfwSetInputMode(VoxagenRenderer::WINDOW, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
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
        VoxagenRenderer::CAMERA.ProcessMouseMovement(xoffset, yoffset);
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

bool point3d(int x, int y, int z, char bt)
{
    int CHUNK_SIZE = WorldVariables::CHUNK_SIZE;
    Chunk* c = VoxagenEngine::CHUNK_MANAGER.ChunkStore.GetChunk(x/CHUNK_SIZE,y/CHUNK_SIZE,z/CHUNK_SIZE);

    //if(c->GetBlock(x%CHUNK_SIZE, y%CHUNK_SIZE, z%CHUNK_SIZE) > 0)
    //{
        if(bt < 1)
        {
            c->RemoveBlock(x%CHUNK_SIZE, y%CHUNK_SIZE, z%CHUNK_SIZE);
        }
        else
        {
            c->AddBlock(x%CHUNK_SIZE, y%CHUNK_SIZE, z%CHUNK_SIZE, bt);
        }

        c->rebuild = true;
        c->isMeshed = false;
        c->isMeshing = false;

        if((x%CHUNK_SIZE < 1 || y%CHUNK_SIZE <1 || z%CHUNK_SIZE < 1) || (x%CHUNK_SIZE >=CHUNK_SIZE-1 || y%CHUNK_SIZE >= CHUNK_SIZE-1 || z%CHUNK_SIZE >= CHUNK_SIZE-1))
        {
            Chunk* top = VoxagenEngine::CHUNK_MANAGER.ChunkStore.GetChunk(c->chunkX, c->chunkY+1, c->chunkZ);
            top->rebuild = true;
            top->isMeshed = false;
            top->isMeshing = false;

            top = VoxagenEngine::CHUNK_MANAGER.ChunkStore.GetChunk(c->chunkX, c->chunkY-1, c->chunkZ);
            top->rebuild = true;
            top->isMeshed = false;
            top->isMeshing = false;

            top = VoxagenEngine::CHUNK_MANAGER.ChunkStore.GetChunk(c->chunkX+1, c->chunkY, c->chunkZ);
            top->rebuild = true;
            top->isMeshed = false;
            top->isMeshing = false;

            top = VoxagenEngine::CHUNK_MANAGER.ChunkStore.GetChunk(c->chunkX-1, c->chunkY, c->chunkZ);
            top->rebuild = true;
            top->isMeshed = false;
            top->isMeshing = false;

            top = VoxagenEngine::CHUNK_MANAGER.ChunkStore.GetChunk(c->chunkX, c->chunkY, c->chunkZ+1);
            top->rebuild = true;
            top->isMeshed = false;
            top->isMeshing = false;

            top = VoxagenEngine::CHUNK_MANAGER.ChunkStore.GetChunk(c->chunkX, c->chunkY, c->chunkZ-1);
            top->rebuild = true;
            top->isMeshed = false;
            top->isMeshing = false;
        }
        return true;
    //}WW
    return false;
}

void raycast(char bt)
{
    int x1 = WorldVariables::CUR_POS_INT.x, y1 = WorldVariables::CUR_POS_INT.y, z1 = WorldVariables::CUR_POS_INT.z;
    int x2 = static_cast<int>(WorldVariables::LOOKING_AT.x), y2 = static_cast<int>(WorldVariables::LOOKING_AT.y), z2 = static_cast<int>(WorldVariables::LOOKING_AT.z);

    int xd, yd, zd;
    int x, y, z;
    int ax, ay, az;
    int sx, sy, sz;
    int dx, dy, dz;

    dx = x2 - x1;
    dy = y2 - y1;
    dz = z2 - z1;

    ax = ABS(dx) << 1;
    ay = ABS(dy) << 1;
    az = ABS(dz) << 1;

    sx = ZSGN(dx);
    sy = ZSGN(dy);
    sz = ZSGN(dz);

    x = x1;
    y = y1;
    z = z1;

    if (ax >= MAX(ay, az))            /* x dominant */
    {
        yd = ay - (ax >> 1);
        zd = az - (ax >> 1);
        for (;;)
        {
            if(point3d(x, y, z, bt))
            {
                return;
            }

            if (x == x2)
            {
                return;
            }

            if (yd >= 0)
            {
                y += sy;
                yd -= ax;
            }

            if (zd >= 0)
            {
                z += sz;
                zd -= ax;
            }

            x += sx;
            yd += ay;
            zd += az;
        }
    }
    else if (ay >= MAX(ax, az))            /* y dominant */
    {
        xd = ax - (ay >> 1);
        zd = az - (ay >> 1);
        for (;;)
        {
            if(point3d(x, y, z, bt))
            {
                return;
            }

            if (y == y2)
            {
                return;
            }

            if (xd >= 0)
            {
                x += sx;
                xd -= ay;
            }

            if (zd >= 0)
            {
                z += sz;
                zd -= ay;
            }

            y += sy;
            xd += ax;
            zd += az;
        }
    }
    else if (az >= MAX(ax, ay))            /* z dominant */
    {
        xd = ax - (az >> 1);
        yd = ay - (az >> 1);
        for (;;)
        {
            if(point3d(x, y, z, bt))
            {
                return;
            }

            if (z == z2)
            {
                return;
            }

            if (xd >= 0)
            {
                x += sx;
                xd -= az;
            }

            if (yd >= 0)
            {
                y += sy;
                yd -= az;
            }

            z += sz;
            xd += ax;
            yd += ay;
        }
    }
}

void InputController::MouseClickCallback(GLFWwindow *window, int button, int action, int mods)
{
    if(button == GLFW_MOUSE_BUTTON_RIGHT || button == GLFW_MOUSE_BUTTON_LEFT)
    {
        if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS)
        {
            //point3d(WorldVariables::LOOKING_AT.x, WorldVariables::LOOKING_AT.y, WorldVariables::LOOKING_AT.z, 3);
        }
        else if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
        {
            //point3d(WorldVariables::LOOKING_AT.x, WorldVariables::LOOKING_AT.y, WorldVariables::LOOKING_AT.z, 0);
        }
    }
}

void InputController::ScrollCallback(GLFWwindow* window, double xoffset, double yoffset)
{
    VoxagenRenderer::CAMERA.ProcessMouseScroll(xoffset);
}

void InputController::ProcessInput(float deltaTime)
{
    if (glfwGetKey(VoxagenRenderer::WINDOW, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(VoxagenRenderer::WINDOW, true);
    if (glfwGetKey(VoxagenRenderer::WINDOW, GLFW_KEY_W) == GLFW_PRESS)
        VoxagenRenderer::CAMERA.ProcessKeyboard(FORWARD, deltaTime);
    if (glfwGetKey(VoxagenRenderer::WINDOW, GLFW_KEY_S) == GLFW_PRESS)
        VoxagenRenderer::CAMERA.ProcessKeyboard(BACKWARD, deltaTime);
    if (glfwGetKey(VoxagenRenderer::WINDOW, GLFW_KEY_A) == GLFW_PRESS)
        VoxagenRenderer::CAMERA.ProcessKeyboard(LEFT, deltaTime);
    if (glfwGetKey(VoxagenRenderer::WINDOW, GLFW_KEY_D) == GLFW_PRESS)
        VoxagenRenderer::CAMERA.ProcessKeyboard(RIGHT, deltaTime);

    if(glfwGetKey(VoxagenRenderer::WINDOW, GLFW_KEY_LEFT_ALT) == GLFW_PRESS)
    {
        if(!free_mouse && !alt_pressed)
        {
            alt_pressed = true;
            free_mouse = true;
            WorldVariables::MENU = true;
            glfwSetInputMode(VoxagenRenderer::WINDOW, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        }
        else if (free_mouse && !alt_pressed)
        {
            free_mouse = false;
            alt_pressed = true;
            glfwSetInputMode(VoxagenRenderer::WINDOW, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        }
    }
    if(glfwGetKey(VoxagenRenderer::WINDOW, GLFW_KEY_LEFT_ALT) == GLFW_RELEASE)
    {
        alt_pressed = false;
    }
    if(glfwGetKey(VoxagenRenderer::WINDOW, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
    {
        VoxagenRenderer::CAMERA.SpeedToggle = true;
    }
    if(glfwGetKey(VoxagenRenderer::WINDOW, GLFW_KEY_LEFT_SHIFT) == GLFW_RELEASE)
    {
        VoxagenRenderer::CAMERA.SpeedToggle = false;
    }
}
