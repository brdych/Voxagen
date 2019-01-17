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
#include "utility/voxagensettings.hpp"
#include "utility/shader.hpp"
#include "utility/chunkmanager.hpp"
#include "utility/inputcontroller.hpp"
#include "worldvariables.hpp"


// settings
const unsigned int SCR_WIDTH = 1920;
const unsigned int SCR_HEIGHT = 1080;

VoxagenSettings* settings = VoxagenSettings::SettingsInstance();
Loader* loader = new Loader();
GLFWwindow* window = loader->LoadGL(SCR_WIDTH, SCR_HEIGHT);
GuiManager* gui = new GuiManager(window);
Camera* camera = new Camera(glm::vec3(0.0f, 0.0f, 20.0f));
InputController* inputController = InputController::GetInputControllerInstance();
ChunkManager* chunkManager = ChunkManager::ChunkManagerInstance();

float deltaTime = 0.0f;
float lastFrame = 0.0f;

void processInput(GLFWwindow *window);

float RandomFloat(float a, float b) {
    return a + ((((float) rand()) / (float) RAND_MAX) * (b - a));
}

int main(void)
{
    cout << "Voxagen Started!" << endl;
    inputController->SetupControls(window, camera);

    glm::mat4 proj = glm::perspective(glm::radians(50.0f), (float)SCR_WIDTH/(float)SCR_HEIGHT, 0.1f, 2000.0f);
    glm::mat4 model = glm::mat4(1);
    glm::mat4 view;
    glm::mat4 mvp;

    Shader light_shader("../src/shaders/LightShader.vert","../src/shaders/LightShader.frag");
    GLint LightMatrixID = glGetUniformLocation(light_shader.ID, "MVP");
    static const GLfloat light_vertices[] = {
        -0.5f,-0.5f,-0.5f, //BLB 0
        -0.5f,-0.5f, 0.5f, //BLF 1
         0.5f,-0.5f,-0.5f, //BRB 2
         0.5f,-0.5f, 0.5f, //BRF 3
        -0.5f, 0.5f,-0.5f, //TLB 4
        -0.5f, 0.5f, 0.5f, //TLF 5
         0.5f, 0.5f,-0.5f, //TRB 6
         0.5f, 0.5f, 0.5f  //TRF 7
    };
    static const GLuint light_indices[] = {
        4,7,6,  //Top
        4,5,7,
        1,2,3,  //Bottom
        1,0,2,
        7,2,6,  //Right
        7,3,2,
        4,1,5,  //Left
        4,0,1,
        6,0,4,  //Back
        6,2,0,
        5,3,7,  //Front
        5,1,3
    };
    GLuint lightVAO, lightVBO, lightEBO;
    glGenVertexArrays(1, &lightVAO);
    glGenBuffers(1, &lightVBO);
    glGenBuffers(1, &lightEBO);

    glBindBuffer(GL_ARRAY_BUFFER, lightVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(light_vertices), light_vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, lightEBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(light_indices), light_indices, GL_STATIC_DRAW);

    glBindVertexArray(lightVAO);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);
    glEnableVertexAttribArray(0);

    std::vector<Chunk*>* chunks = new std::vector<Chunk*>();
    for(int x = -10; x < 10; x++)
        for (int z = -10; z < 10; z++)
            for(int y = 1; y < 4; y++)
            {
                Chunk* c = new Chunk(x,y,z);
                c->BuildVoxelData();
                c->BuildChunkMesh();
                chunks->push_back(c);
            }

    /*chunks->push_back(new Chunk(0,0,0));
    for(Chunk* c: *chunks)
    {
        c->BuildVoxelData();
        c->BuildChunkMesh();
    }*/


    //std::thread t_ChunkManager(UpdateLoop);

    while(!glfwWindowShouldClose(window)&&!settings->PROGRAM_SHOULD_EXIT)
    {
        // Input
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        inputController->ProcessInput(deltaTime);

        glm::vec4* clear_color = settings->CLEAR_COLOUR;
        glClearColor(clear_color->x, clear_color->y, clear_color->z, clear_color->w);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glDepthFunc(GL_LESS);

        if(settings->CULLING_ENABLED) { glEnable(GL_CULL_FACE); }
        else { glDisable(GL_CULL_FACE); }
        if(settings->Z_BUFFER_ENABLED) { glEnable(GL_DEPTH_TEST); }
        else { glDisable(GL_DEPTH_TEST); }
        if(settings->USE_WIREFRAME) { glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); }
        else { glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); }

        //Update View
        view = camera->GetViewMatrix();

        //Draw Light
        light_shader.use();
        glm::mat4 model = glm::translate(glm::mat4(1), *settings->LIGHT_POS);
        mvp = proj * view * model;
        glUniformMatrix4fv(LightMatrixID, 1, GL_FALSE, &mvp[0][0]);
        glBindVertexArray(lightVAO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, lightEBO);
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, nullptr);

        //Draw Chunks
        for(Chunk* c: *chunks)
        {
            VoxelRenderer::SetupShader();
            c->Render(&view,&proj,&mvp);
            if(settings->SHOW_CHUNK_BOUNDS || (settings->ONLY_RENDERING_CHUNKS && c->ShouldRender(0, nullptr, nullptr)))
            {
                light_shader.use();
                glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
                glm::mat4 model = glm::translate(glm::mat4(1), glm::vec3((c->chunkX*Chunk::CHUNK_SIZE)-0.5,(c->chunkY*Chunk::CHUNK_SIZE)-0.5,(c->chunkZ*Chunk::CHUNK_SIZE)-0.5));
                model = glm::translate(model, glm::vec3(Chunk::CHUNK_SIZE/2,Chunk::CHUNK_SIZE/2,Chunk::CHUNK_SIZE/2));
                model = glm::scale(model, glm::vec3(Chunk::CHUNK_SIZE,Chunk::CHUNK_SIZE,Chunk::CHUNK_SIZE));
                mvp = proj * view * model;
                glUniformMatrix4fv(LightMatrixID, 1, GL_FALSE, &mvp[0][0]);
                glBindVertexArray(lightVAO);
                glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, lightEBO);
                glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, nullptr);
                glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
            }
        }

        // Start the Dear ImGui frame
        if(settings->MENU)
        {
            gui->drawControlPanel(camera);
        }

        glfwMakeContextCurrent(window);
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    chunkManager->Shutdown = true;
    //t_ChunkManager.join();
    cout << "Shutting Down Voxagen!" << endl;
    return 0;
}

void SetupLightShader()
{

}
