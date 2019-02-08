#include <GLFW/glfw3.h>
#include <iostream>
#include <cstdio>
#include <glm/gtc/matrix_transform.hpp>

#include "worldvariables.hpp"
#include "utility/voxagenrenderer.hpp"
#include "utility/voxagenengine.hpp"

int main()
{
    cout << "Voxagen Started!" << endl;
    while(!glfwWindowShouldClose(VoxagenRenderer::WINDOW)&&!WorldVariables::PROGRAM_SHOULD_EXIT)
    {
        // Input
        float currentFrame = static_cast<float>(glfwGetTime());
        WorldVariables::DELTA_TIME = currentFrame - WorldVariables::LAST_FRAME;
        WorldVariables::LAST_FRAME = currentFrame;

        glm::vec3 clear_color = WorldVariables::CLEAR_COLOUR;
        glClearColor(clear_color.x, clear_color.y, clear_color.z, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glDepthFunc(GL_LESS);

        // Update Controller
        VoxagenEngine::INPUT_MANAGER.ProcessInput(WorldVariables::DELTA_TIME);

        //Update View
        VoxagenRenderer::VIEW_MAT = VoxagenRenderer::CAMERA.GetViewMatrix();

        glm::vec3 pos = VoxagenRenderer::CAMERA.Position;
        int cs = static_cast<int>(WorldVariables::CHUNK_SIZE);
        int ix = static_cast<int>(pos.x), iy = static_cast<int>(pos.y), iz = static_cast<int>(pos.z);
        WorldVariables::CUR_POS = pos;
        WorldVariables::CUR_POS_INT = glm::vec3(ix, iy, iz);
        WorldVariables::CUR_CHUNK = glm::vec3(ix/cs, iy/cs, iz/cs);
        WorldVariables::CUR_POS_CHUNK = glm::vec3(abs(ix%cs), abs(iy%cs), abs(iz%cs));
        if(pos.x<0)
        {
            WorldVariables::CUR_CHUNK.x-=1;
            WorldVariables::CUR_POS_CHUNK.x = WorldVariables::CHUNK_SIZE - WorldVariables::CUR_POS_CHUNK.x;
        }
        if(pos.y<0)
        {
            WorldVariables::CUR_CHUNK.y-=1;
            WorldVariables::CUR_POS_CHUNK.y = WorldVariables::CHUNK_SIZE - WorldVariables::CUR_POS_CHUNK.y;
        }
        if(pos.z<0)
        {
            WorldVariables::CUR_CHUNK.z-=1;
            WorldVariables::CUR_POS_CHUNK.z = WorldVariables::CHUNK_SIZE - WorldVariables::CUR_POS_CHUNK.z;
        }

        //d.DrawDebugCube(pos.x,pos.y,pos.z,proj,view);

        // Update Chunks
        VoxagenEngine::CHUNK_MANAGER.Update(&VoxagenRenderer::CAMERA,0);

        // Render Chunks
        VoxagenEngine::CHUNK_MANAGER.Render(&VoxagenRenderer::VIEW_MAT, &VoxagenRenderer::PROJ_MAT, &VoxagenRenderer::MVP_MAT);

        // Render Dear ImGui
        if(WorldVariables::MENU) { VoxagenEngine::GUI_MANAGER.drawControlPanel(&VoxagenRenderer::CAMERA); }

        // Clear RenderList for next frame
        VoxagenEngine::CHUNK_MANAGER.ClearRenderList();

        glfwSwapBuffers(VoxagenRenderer::WINDOW);
        glfwPollEvents();
    }
    cout << "Shutting Down Voxagen!" << endl;
    return 0;
}
