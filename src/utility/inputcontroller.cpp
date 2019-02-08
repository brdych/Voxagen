#include "inputcontroller.hpp"

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

void InputController::MouseClickCallback(GLFWwindow *window, int button, int action, int mods)
{
    int CHUNK_SIZE = WorldVariables::CHUNK_SIZE;
    glm::vec3 cp = WorldVariables::CUR_CHUNK;
    glm::vec3 cb = WorldVariables::CUR_POS_CHUNK;
    if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS)
    {
            //calculate angle vector of the mouse
            double cursorX, cursorY;
            glfwGetCursorPos(window, &cursorX, &cursorY);
            float x = (2.0f * cursorX) / 1280.0f - 1.0f;
            float y = 1.0f - (2.0f * cursorY) / 720.0f;
            std::cout << "Cursor: " << cursorX << ", " << cursorY << std::endl;

            glm::vec4 rayClip = glm::vec4(x, y, -1.0, 1.0);
            /*glm::vec4 rayEye = glm::inverse(projectionMatrix) * rayClip;
            rayEye = vec4 (rayEye.v[0], rayEye.v[1], -1.0f, 0.0f);
            glm::vec3 rayWOR = vec4(inverse(camera->getMatrix()) * rayEye);
            rayWOR = glm::normalise(rayWOR);

            float range = 64.0f; //max range to check (in voxels)
            glm::vec3 camPos = camera->getPosition() * (1 / voxelSize);
            float xPos = floor(camPos.v[0]);
            float yPos = floor(camPos.v[1]);
            float zPos = floor(camPos.v[2]);
            int stepX = signum(rayWOR.v[0]);
            int stepY = signum(rayWOR.v[1]);
            int stepZ = signum(rayWOR.v[2]);
            glm::vec3 tMax(intbound(camPos.v[0], rayWOR.v[0]), intbound(camPos.v[1], rayWOR.v[1]), intbound(camPos.v[2], rayWOR.v[2]));
            glm::vec3 tDelta((float)stepX / rayWOR.v[0], (float)stepY / rayWOR.v[1], (float)stepZ / rayWOR.v[2]);
            float faceX;
            float faceY;
            float faceZ;

            std::cout << "Camera Pos: " << camPos.x << " " << camPos.y << " " << camPos.z << std::endl;

            do {
                if (isVoxelSolid(xPos,yPos,zPos)) {
                    std::cout << "boom";
                    if (button == GLFW_MOUSE_BUTTON_2) setVoxelType(xPos,yPos,zPos, 0, true);
                    else setVoxelType(xPos + faceX, yPos + faceY, zPos + faceZ, 1, true);
                    break;
                }
                if (tMax.v[0] < tMax.v[1]) {
                    if (tMax.v[0] < tMax.v[2]) {
                        if (tMax.v[0] > range) break;

                        xPos += stepX;
                        tMax.v[0] += tDelta.v[0];

                        faceX = -stepX;
                        faceY = 0;
                        faceZ = 0;
                    } else {
                        if (tMax.v[2] > range) break;
                        zPos += stepZ;
                        tMax.v[2] += tDelta.v[2];
                        faceX = 0;
                        faceY = 0;
                        faceZ = -stepZ;
                    }
                } else {
                    if (tMax.v[1] < tMax.v[2]) {
                        if (tMax.v[1] > range) break;
                        yPos += stepY;
                        tMax.v[1] += tDelta.v[1];
                        faceX = 0;
                        faceY = -stepY;
                        faceZ = 0;
                    } else {
                        if (tMax.v[2] > range) break;
                        zPos += stepZ;
                        tMax.v[2] += tDelta.v[2];
                        faceX = 0;
                        faceY = 0;
                        faceZ = -stepZ;
                    }
                }
            } while (true);
        }*/
    }
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
    {
        /*Chunk* c = VoxagenEngine::CHUNK_MANAGER.ChunkStore->GetChunk(cp.x,cp.y,cp.z);
        if(c->isMeshed)
        {
            c->RemoveBlock(cb.x,cb.y,cb.z);
            c->isMeshed = false;
            c->isMeshing = false;
        }*/
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
