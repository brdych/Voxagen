#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <stdio.h>

using namespace std;

#include "gui/guimanager.hpp"
#include "world/chunk.hpp"
#include "utility/camera.hpp"
#include "utility/loader.hpp"
#include "utility/voxagensettings.hpp"
#include "utility/shader.hpp"
#include "utility/chunkmanager.hpp"


void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow *window);

// settings
const unsigned int SCR_WIDTH = 1920;
const unsigned int SCR_HEIGHT = 1080;

VoxagenSettings* settings = VoxagenSettings::SettingsInstance();

Loader* loader = new Loader();
GLFWwindow* window = loader->loadGL(SCR_WIDTH, SCR_HEIGHT);
GuiManager* gui = new GuiManager(window);
Camera* camera = new Camera(glm::vec3(0.0f, 0.0f, 20.0f));

float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;
float deltaTime = 0.0f;
float lastFrame = 0.0f;
bool free_mouse = false;
bool alt_pressed = false;

float RandomFloat(float a, float b) {
    return a + ((((float) rand()) / (float) RAND_MAX) * (b - a));
}

int main(void)
{
    cout << "Hello world!" << endl;
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    VoxagenSettings*vs = VoxagenSettings::SettingsInstance();
    ChunkManager* cm = ChunkManager::ChunkManagerInstance();
    cm->CreateChunks();
    cm->BuildMeshes();

    Shader block_shader("../src/shaders/BlockShader.vert","../src/shaders/BlockShader.frag");
    GLint MatrixID = glGetUniformLocation(block_shader.ID, "MVP");

    Shader light_shader("../src/shaders/LightShader.vert","../src/shaders/LightShader.frag");
    GLint LightMatrixID = glGetUniformLocation(light_shader.ID, "MVP");

    glm::mat4 proj = glm::perspective(glm::radians(45.0f), (float)SCR_WIDTH/(float)SCR_HEIGHT, 0.1f, 1000.0f);
    glm::mat4 model = glm::mat4(1);
    glm::mat4 view;
    glm::mat4 mvp;

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


    while(!glfwWindowShouldClose(window)&&!settings->PROGRAM_SHOULD_EXIT)
    {
        // Input
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        processInput(window);

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
        block_shader.use();
        block_shader.setVec3("GLOBAL_LIGHT_COL", *settings->GLOBAL_LIGHT_COL);
        block_shader.setVec3("GLOBAL_LIGHT_DIR", *settings->GLOBAL_LIGHT_DIR);
        for(uint x = 0; x < cm->num_chunks_X; x++)
            for(uint y = 0; y < cm->num_chunks_Y; y++)
                for(uint z = 0; z < cm->num_chunks_Z; z++)
                {
                    if(cm->chunks[x][y][z]->ShouldRender())
                    {
                        model = glm::translate(glm::mat4(1), glm::vec3(x*Chunk::CHUNK_SIZE,y*Chunk::CHUNK_SIZE,z*Chunk::CHUNK_SIZE));
                        mvp = proj * view * model;
                        glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &mvp[0][0]);
                        cm->chunks[x][y][z]->Render();
                    }
                }

        // Start the Dear ImGui frame
        gui->drawControlPanel(camera);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwMakeContextCurrent(window);
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    cout << "Goodbye world!" << endl;
    return 0;
}

void processInput(GLFWwindow *window)
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

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
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

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    camera->ProcessMouseScroll(xoffset);
}
