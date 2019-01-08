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
Camera* camera = new Camera(glm::vec3(0.0f, 0.0f, 3.0f));

float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;
float deltaTime = 0.0f;
float lastFrame = 0.0f;
bool free_mouse = false;
bool alt_pressed = false;

const static int chunk_size = 32;
const static GLfloat voxel_size = 0.5f;

std::vector<GLfloat>* cvs = new std::vector<GLfloat>();

struct Voxel{
    bool enabled;
    glm::vec3 col;
};

Voxel voxeldata[chunk_size][chunk_size][chunk_size];

bool RandomBool() {
   return rand() > (RAND_MAX / 2);
}

float RandomFloat(float a, float b) {
    return a + ((((float) rand()) / (float) RAND_MAX) * (b - a));
}

void generateVoxelData()
{
    cout <<"Creating random voxel data" << endl;
    for(int x = 0; x < chunk_size; x++)
    {
        for(int y = 0; y < chunk_size; y++)
        {
            for(int z = 0; z < chunk_size; z++)
            {
                Voxel v;
                v.enabled = RandomBool();
                v.col = glm::vec3(RandomFloat(0,1),RandomFloat(0,1),RandomFloat(0,1));
                voxeldata[x][y][z] = v;
            }
        }
    }
    cout <<"Voxel data created!" << endl;
}

void createCube(int x, int y, int z)
{
    glm::vec3 v0 = glm::vec3(x-voxel_size,y-voxel_size,z-voxel_size);
    glm::vec3 v1 = glm::vec3(x-voxel_size,y-voxel_size,z+voxel_size);
    glm::vec3 v2 = glm::vec3(x+voxel_size,y-voxel_size,z-voxel_size);
    glm::vec3 v3 = glm::vec3(x+voxel_size,y-voxel_size,z+voxel_size);
    glm::vec3 v4 = glm::vec3(x-voxel_size,y+voxel_size,z-voxel_size);
    glm::vec3 v5 = glm::vec3(x-voxel_size,y+voxel_size,z+voxel_size);
    glm::vec3 v6 = glm::vec3(x+voxel_size,y+voxel_size,z-voxel_size);
    glm::vec3 v7 = glm::vec3(x+voxel_size,y+voxel_size,z+voxel_size);

    //Top
    cvs->push_back(v4.x);
    cvs->push_back(v4.y);
    cvs->push_back(v4.z);

    cvs->push_back(v7.x);
    cvs->push_back(v7.y);
    cvs->push_back(v7.z);

    cvs->push_back(v6.x);
    cvs->push_back(v6.y);
    cvs->push_back(v6.z);

    cvs->push_back(v4.x);
    cvs->push_back(v4.y);
    cvs->push_back(v4.z);

    cvs->push_back(v5.x);
    cvs->push_back(v5.y);
    cvs->push_back(v5.z);

    cvs->push_back(v7.x);
    cvs->push_back(v7.y);
    cvs->push_back(v7.z);

    //Bottom
    cvs->push_back(v1.x);
    cvs->push_back(v1.y);
    cvs->push_back(v1.z);

    cvs->push_back(v2.x);
    cvs->push_back(v2.y);
    cvs->push_back(v2.z);

    cvs->push_back(v3.x);
    cvs->push_back(v3.y);
    cvs->push_back(v3.z);

    cvs->push_back(v1.x);
    cvs->push_back(v1.y);
    cvs->push_back(v1.z);

    cvs->push_back(v0.x);
    cvs->push_back(v0.y);
    cvs->push_back(v0.z);

    cvs->push_back(v2.x);
    cvs->push_back(v2.y);
    cvs->push_back(v2.z);

    //Right
    cvs->push_back(v7.x);
    cvs->push_back(v7.y);
    cvs->push_back(v7.z);

    cvs->push_back(v2.x);
    cvs->push_back(v2.y);
    cvs->push_back(v2.z);

    cvs->push_back(v6.x);
    cvs->push_back(v6.y);
    cvs->push_back(v6.z);

    cvs->push_back(v7.x);
    cvs->push_back(v7.y);
    cvs->push_back(v7.z);

    cvs->push_back(v3.x);
    cvs->push_back(v3.y);
    cvs->push_back(v3.z);

    cvs->push_back(v2.x);
    cvs->push_back(v2.y);
    cvs->push_back(v2.z);

    //Left
    cvs->push_back(v4.x);
    cvs->push_back(v4.y);
    cvs->push_back(v4.z);

    cvs->push_back(v1.x);
    cvs->push_back(v1.y);
    cvs->push_back(v1.z);

    cvs->push_back(v5.x);
    cvs->push_back(v5.y);
    cvs->push_back(v5.z);

    cvs->push_back(v4.x);
    cvs->push_back(v4.y);
    cvs->push_back(v4.z);

    cvs->push_back(v0.x);
    cvs->push_back(v0.y);
    cvs->push_back(v0.z);

    cvs->push_back(v1.x);
    cvs->push_back(v1.y);
    cvs->push_back(v1.z);

    //Back
    cvs->push_back(v6.x);
    cvs->push_back(v6.y);
    cvs->push_back(v6.z);

    cvs->push_back(v0.x);
    cvs->push_back(v0.y);
    cvs->push_back(v0.z);

    cvs->push_back(v4.x);
    cvs->push_back(v4.y);
    cvs->push_back(v4.z);

    cvs->push_back(v6.x);
    cvs->push_back(v6.y);
    cvs->push_back(v6.z);

    cvs->push_back(v2.x);
    cvs->push_back(v2.y);
    cvs->push_back(v2.z);

    cvs->push_back(v0.x);
    cvs->push_back(v0.y);
    cvs->push_back(v0.z);

    //Front
    cvs->push_back(v5.x);
    cvs->push_back(v5.y);
    cvs->push_back(v5.z);

    cvs->push_back(v3.x);
    cvs->push_back(v3.y);
    cvs->push_back(v3.z);

    cvs->push_back(v7.x);
    cvs->push_back(v7.y);
    cvs->push_back(v7.z);

    cvs->push_back(v5.x);
    cvs->push_back(v5.y);
    cvs->push_back(v5.z);

    cvs->push_back(v1.x);
    cvs->push_back(v1.y);
    cvs->push_back(v1.z);

    cvs->push_back(v3.x);
    cvs->push_back(v3.y);
    cvs->push_back(v3.z);
}

int main(void)
{
    generateVoxelData();
    cout << "Hello world!" << endl;
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    static const GLfloat voxel_vertices[] = {
        -voxel_size,-voxel_size,-voxel_size, //BLB 0
        -voxel_size,-voxel_size, voxel_size, //BLF 1
         voxel_size,-voxel_size,-voxel_size, //BRB 2
         voxel_size,-voxel_size, voxel_size, //BRF 3
        -voxel_size, voxel_size,-voxel_size, //TLB 4
        -voxel_size, voxel_size, voxel_size, //TLF 5
         voxel_size, voxel_size,-voxel_size, //TRB 6
         voxel_size, voxel_size, voxel_size  //TRF 7
    };

    static const GLuint voxel_indices[] = {
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

    for(int z = 0; z < chunk_size; z++)
    {
        for(int y = 0; y < chunk_size; y++)
        {
            for(int x = 0; x < chunk_size; x++)
            {
                if(voxeldata[x][y][z].enabled)
                {
                    createCube(x,y,z);
                }
            }
        }
    }

    //VAO
    GLuint vertexarray;
    glGenVertexArrays(1, &vertexarray);
    glBindVertexArray(vertexarray);
    //VBO
    GLuint vertexbuffer;
    glGenBuffers(1, &vertexbuffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
    glBufferData(GL_ARRAY_BUFFER, cvs->size()*sizeof(GLfloat), cvs->data(), GL_STATIC_DRAW);
    //EBO
    //GLuint elementbuffer;
    //glGenBuffers(1, &elementbuffer);
    //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementbuffer);
    //glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(voxel_indices), voxel_indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);
    glEnableVertexAttribArray(0);

    Shader shader_programme("../src/shaders/TestShader.vert","../src/shaders/TestShader.frag");
    GLuint MatrixID = glGetUniformLocation(shader_programme.ID, "MVP");
    GLuint VoxelColorID = glGetUniformLocation(shader_programme.ID, "VOXEL_COLOR");

    glm::mat4 proj = glm::perspective(glm::radians(45.0f), (float)SCR_WIDTH/(float)SCR_HEIGHT, 0.1f, 100.0f);
    glm::mat4 view;
    glm::mat4 model;
    glm::mat4 mvp;

    while(!glfwWindowShouldClose(window)&&!settings->PROGRAM_SHOULD_EXIT)
    {
        // Input
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        processInput(window);

        // Start the Dear ImGui frame
        gui->drawControlPanel();
        
        // Rendering
        int display_w, display_h;
        glfwMakeContextCurrent(window);
        glfwGetFramebufferSize(window, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);

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

        if(settings->REGEN_VOXELDATA)
        {
            generateVoxelData();
            settings->REGEN_VOXELDATA = false;
        }

        cout << cvs->size()/3 << endl;

        view = camera->GetViewMatrix();
        shader_programme.use();
        shader_programme.setVec3("VOXEL_COLOR", glm::vec3(0.5,0.5,0.5));
        model = glm::mat4(1);
        mvp = proj * view * model;
        glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &mvp[0][0]);
        glDrawArrays(GL_TRIANGLES, 0, cvs->size()/3);

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
    camera->ProcessMouseScroll(yoffset);
}
