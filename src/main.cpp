#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <cstdio>
#include <thread>
#include <unordered_map>

#include "gui/guimanager.hpp"
#include "world/chunk.hpp"
#include "utility/camera.hpp"
#include "utility/loader.hpp"
#include "utility/chunkmanager.hpp"
#include "utility/inputcontroller.hpp"
#include "worldvariables.hpp"
#include "utility/debug.hpp"
#include "world/generation/worldgenerator.hpp"
#include "world/generation/terraingenerator.hpp"
#include "utility/chunkstorage.hpp"

// settings
const unsigned int SCR_WIDTH = 1920;
const unsigned int SCR_HEIGHT = 1080;

int main()
{
    cout << "Voxagen Started!" << endl;

    Loader* loader = new Loader();
    GLFWwindow* window = loader->LoadGL(SCR_WIDTH, SCR_HEIGHT);
    GuiManager* gui = new GuiManager(window);
    Camera* camera = new Camera(glm::vec3(0.0f, 100.0f, 0.0f));
    InputController* inputController = InputController::GetInputControllerInstance();
    ChunkManager* chunkManager = ChunkManager::ChunkManagerInstance();

    glm::mat4 proj = glm::perspective(glm::radians(50.0f), (float)SCR_WIDTH/(float)SCR_HEIGHT, 0.1f, 2000.0f);
    glm::mat4 view;
    glm::mat4 mvp;
    DebugObject d = DebugObject();

    inputController->SetupControls(window, camera);

    while(!glfwWindowShouldClose(window)&&!WorldVariables::PROGRAM_SHOULD_EXIT)
    {
        // Input
        float currentFrame = (float)glfwGetTime();
        WorldVariables::DELTA_TIME = currentFrame - WorldVariables::LAST_FRAME;
        WorldVariables::LAST_FRAME = currentFrame;
        glm::vec3* clear_color = WorldVariables::CLEAR_COLOUR;

        glClearColor(clear_color->x, clear_color->y, clear_color->z, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glDepthFunc(GL_LESS);

        // Update Controller
        inputController->ProcessInput(WorldVariables::DELTA_TIME);

        //Update View
        view = camera->GetViewMatrix();

        glm::vec3 pos = camera->Position;
        int cs = WorldVariables::CHUNK_SIZE;
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
        chunkManager->Update(camera,0);
        // Render Chunks
        chunkManager->Render(&view,&proj,&mvp);

        // Render Dear ImGui
        if(WorldVariables::MENU) { gui->drawControlPanel(camera); }

        // Clear RenderList for next frame
        chunkManager->ClearRenderList();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    //chunkManager->Shutdown();
    cout << "Beginning Shutdown" << endl;

    cout << "Deleting ChunkManager" << endl;
    delete chunkManager;

    cout << "Deleting Loader" << endl;
    delete loader;

    //delete gui;

    cout << "Deleting Camera" << endl;
    delete camera;

    cout << "Deleting Input Controller" << endl;
    delete inputController;

    cout << "Shutting Down Voxagen!" << endl;
    return 0;
}
