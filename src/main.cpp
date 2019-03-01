#include <GLFW/glfw3.h>
#include <iostream>
#include <cstdio>
#include <glm/gtc/matrix_transform.hpp>

#include "worldvariables.hpp"
#include "utility/Voxagen/voxagenrenderer.hpp"
#include "utility/Voxagen/voxagenengine.hpp"

int main()
{
    std::cout << "Voxagen Started!" << std::endl;
    while(!glfwWindowShouldClose(VoxagenRenderer::WINDOW)&&!WorldVariables::PROGRAM_SHOULD_EXIT)
    {
        // Input
        float currentFrame = static_cast<float>(glfwGetTime());
        WorldVariables::DELTA_TIME = currentFrame - WorldVariables::LAST_FRAME;
        WorldVariables::LAST_FRAME = currentFrame;

        glClearColor(WorldVariables::CLEAR_COLOUR.x, WorldVariables::CLEAR_COLOUR.y, WorldVariables::CLEAR_COLOUR.z, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glDepthFunc(GL_LESS);


        // Update Controller
        VoxagenEngine::INPUT_MANAGER.ProcessInput(WorldVariables::DELTA_TIME);

        // Update View
        VoxagenRenderer::VIEW_MAT = VoxagenRenderer::CAMERA.GetViewMatrix();

        // Update Position Variables
        WorldVariables::CUR_POS = VoxagenRenderer::CAMERA.Position;
        WorldVariables::CUR_POS_INT = glm::vec3(static_cast<int>(VoxagenRenderer::CAMERA.Position.x), static_cast<int>(VoxagenRenderer::CAMERA.Position.y), static_cast<int>(VoxagenRenderer::CAMERA.Position.z));
        WorldVariables::CUR_CHUNK = glm::vec3(static_cast<int>(VoxagenRenderer::CAMERA.Position.x)/static_cast<int>(WorldVariables::CHUNK_SIZE), static_cast<int>(VoxagenRenderer::CAMERA.Position.y)/static_cast<int>(WorldVariables::CHUNK_SIZE), static_cast<int>(VoxagenRenderer::CAMERA.Position.z)/static_cast<int>(WorldVariables::CHUNK_SIZE));
        WorldVariables::CUR_POS_CHUNK = glm::vec3(abs(static_cast<int>(VoxagenRenderer::CAMERA.Position.x)%static_cast<int>(WorldVariables::CHUNK_SIZE)), abs(static_cast<int>(VoxagenRenderer::CAMERA.Position.y)%static_cast<int>(WorldVariables::CHUNK_SIZE)), abs(static_cast<int>(VoxagenRenderer::CAMERA.Position.z)%static_cast<int>(WorldVariables::CHUNK_SIZE)));
        if(VoxagenRenderer::CAMERA.Position.x<0)
        {
            WorldVariables::CUR_CHUNK.x-=1;
            WorldVariables::CUR_POS_CHUNK.x = WorldVariables::CHUNK_SIZE - WorldVariables::CUR_POS_CHUNK.x;
        }
        if(VoxagenRenderer::CAMERA.Position.y<0)
        {
            WorldVariables::CUR_CHUNK.y-=1;
            WorldVariables::CUR_POS_CHUNK.y = WorldVariables::CHUNK_SIZE - WorldVariables::CUR_POS_CHUNK.y;
        }
        if(VoxagenRenderer::CAMERA.Position.z<0)
        {
            WorldVariables::CUR_CHUNK.z-=1;
            WorldVariables::CUR_POS_CHUNK.z = WorldVariables::CHUNK_SIZE - WorldVariables::CUR_POS_CHUNK.z;
        }
        WorldVariables::LOOKING_AT = static_cast<glm::vec3>(WorldVariables::CUR_POS_INT) + VoxagenRenderer::CAMERA.Front * 8.0f;

        // Update Chunks
        VoxagenEngine::CHUNK_MANAGER.Update();

        // Render Chunks
        VoxagenEngine::CHUNK_MANAGER.Render(&VoxagenRenderer::VIEW_MAT, &VoxagenRenderer::PROJ_MAT, &VoxagenRenderer::MVP_MAT);

        // Render Dear ImGui
        if(WorldVariables::MENU) { VoxagenEngine::GUI_MANAGER.drawControlPanel(); }

        // Clear RenderList for next frame
        VoxagenEngine::CHUNK_MANAGER.ClearRenderList();

        glfwSwapBuffers(VoxagenRenderer::WINDOW);
        glfwPollEvents();
    }
    std::cout << "Shutting Down Voxagen!" << std::endl;
    return 0;
}
