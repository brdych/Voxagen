#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <stdio.h>
#include <thread>

using namespace std;

#include "gui/guimanager.hpp"
#include "world/chunk.hpp"
#include "utility/camera.hpp"
#include "utility/loader.hpp"
#include "utility/chunkmanager.hpp"
#include "utility/inputcontroller.hpp"
#include "worldvariables.hpp"
#include "utility/debug.hpp"


// settings
const unsigned int SCR_WIDTH = 1920;
const unsigned int SCR_HEIGHT = 1080;

float RandomFloat(float a, float b) {
    return a + ((((float) rand()) / (float) RAND_MAX) * (b - a));
}

int main(void)
{
    cout << "Voxagen Started!" << endl;

    Loader* loader = new Loader();
    GLFWwindow* window = loader->LoadGL(SCR_WIDTH, SCR_HEIGHT);
    GuiManager* gui = new GuiManager(window);
    Camera* camera = new Camera(glm::vec3(0.0f, 256.0f, 0.0f));
    InputController* inputController = InputController::GetInputControllerInstance();
    ChunkManager* chunkManager = ChunkManager::ChunkManagerInstance();

    glm::mat4 proj = glm::perspective(glm::radians(50.0f), (float)SCR_WIDTH/(float)SCR_HEIGHT, 0.1f, 2000.0f);
    glm::mat4 view;
    glm::mat4 mvp;

    inputController->SetupControls(window, camera);
    chunkManager->RequestChunks();
    chunkManager->Start();

    while(!glfwWindowShouldClose(window)&&!WorldVariables::PROGRAM_SHOULD_EXIT)
    {
        // Input
        float currentFrame = (float)glfwGetTime();
        WorldVariables::DELTA_TIME = currentFrame - WorldVariables::LAST_FRAME;
        WorldVariables::LAST_FRAME = currentFrame;
        glm::vec4* clear_color = WorldVariables::CLEAR_COLOUR;
        glClearColor(clear_color->x, clear_color->y, clear_color->z, clear_color->w);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glDepthFunc(GL_LESS);

        // Update Controller
        inputController->ProcessInput(WorldVariables::DELTA_TIME);

        //Update View
        view = camera->GetViewMatrix();

        // Render Chunks
        chunkManager->Render(&view,&proj,&mvp);

        // Render Dear ImGui
        if(WorldVariables::MENU) { gui->drawControlPanel(camera); }

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    chunkManager->Shutdown();
    cout << "Shutting Down Voxagen!" << endl;
    return 0;
}
