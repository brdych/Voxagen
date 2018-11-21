#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <stdio.h>

using namespace std;

#include "gui/guimanager.hpp"
#include "world/chunk.hpp"
#include "utility/camera.hpp"
#include "utility/loader.hpp"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow *window);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

Loader* loader = new Loader();
GLFWwindow* window = loader->loadGL(SCR_WIDTH, SCR_HEIGHT);
GuiManager* gui = new GuiManager(window);
Camera* camera = new Camera(glm::vec3(0.0f, 0.0f, 3.0f));
ImVec4 clear_color = ImVec4(0.3f, 0.8f, 1.0f, 1.00f);
ImVec4 cube_color = ImVec4(0.5f, 0.5f, 0.5f, 1.00f);

float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;
float deltaTime = 0.0f;
float lastFrame = 0.0f;

bool program_exit = false;
bool free_mouse = false;
bool alt_pressed = false;
bool culling = false;
bool z_buffer = false;
bool use_cube_color = false;

bool update_color_buffer = false;

int main(void)
{
    cout << "Hello world!" << endl;

    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    //VAO
    GLuint VertexArrayID;
    glGenVertexArrays(1, &VertexArrayID);
    glBindVertexArray(VertexArrayID);

    static const GLfloat g_vertex_buffer_data[] = {
        -1.0f,-1.0f,-1.0f, // triangle 1 : begin
        -1.0f,-1.0f, 1.0f,
        -1.0f, 1.0f, 1.0f, // triangle 1 : end
        1.0f, 1.0f,-1.0f, // triangle 2 : begin
        -1.0f,-1.0f,-1.0f,
        -1.0f, 1.0f,-1.0f, // triangle 2 : end
        1.0f,-1.0f, 1.0f,
        -1.0f,-1.0f,-1.0f,
        1.0f,-1.0f,-1.0f,
        1.0f, 1.0f,-1.0f,
        1.0f,-1.0f,-1.0f,
        -1.0f,-1.0f,-1.0f,
        -1.0f,-1.0f,-1.0f,
        -1.0f, 1.0f, 1.0f,
        -1.0f, 1.0f,-1.0f,
        1.0f,-1.0f, 1.0f,
        -1.0f,-1.0f, 1.0f,
        -1.0f,-1.0f,-1.0f,
        -1.0f, 1.0f, 1.0f,
        -1.0f,-1.0f, 1.0f,
        1.0f,-1.0f, 1.0f,
        1.0f, 1.0f, 1.0f,
        1.0f,-1.0f,-1.0f,
        1.0f, 1.0f,-1.0f,
        1.0f,-1.0f,-1.0f,
        1.0f, 1.0f, 1.0f,
        1.0f,-1.0f, 1.0f,
        1.0f, 1.0f, 1.0f,
        1.0f, 1.0f,-1.0f,
        -1.0f, 1.0f,-1.0f,
        1.0f, 1.0f, 1.0f,
        -1.0f, 1.0f,-1.0f,
        -1.0f, 1.0f, 1.0f,
        1.0f, 1.0f, 1.0f,
        -1.0f, 1.0f, 1.0f,
        1.0f,-1.0f, 1.0f
    };

    static GLfloat g_solid_color_buffer_data[36*3];

    GLuint vertexbuffer;
    // Generate 1 buffer, put the resulting identifier in vertexbuffer
    glGenBuffers(1, &vertexbuffer);
    // The following commands will talk about our 'vertexbuffer' buffer
    glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
    // Give our vertices to OpenGL.
    glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);

    GLuint colorbuffer;
    glGenBuffers(1, &colorbuffer);
    glBindBuffer(GL_ARRAY_BUFFER, colorbuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(g_solid_color_buffer_data), g_solid_color_buffer_data, GL_STATIC_DRAW);

    const char* vertex_shader =
    "#version 400\n"
    "layout(location = 0) in vec3 vertexPosition_modelspace;"
    "layout(location = 1) in vec3 vertexColor;"
    "out vec3 fragmentColor;"
    "uniform mat4 MVP;"
    "void main() {"
    "   gl_Position = MVP * vec4(vertexPosition_modelspace,1);"
    "   fragmentColor = vertexColor;"
    "}";

    const char* fragment_shader =
    "#version 400\n"
    "in vec3 fragmentColor;"
    "out vec3 colour;"
    "void main() {"
    "  colour = fragmentColor;"
    "}";

    GLuint vs = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vs, 1, &vertex_shader, nullptr);
    glCompileShader(vs);

    GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fs, 1, &fragment_shader, nullptr);
    glCompileShader(fs);

    GLuint shader_programme = glCreateProgram();
    glAttachShader(shader_programme, fs);
    glAttachShader(shader_programme, vs);
    glLinkProgram(shader_programme);

    GLuint MatrixID = glGetUniformLocation(shader_programme, "MVP");

    while(!glfwWindowShouldClose(window)&&!program_exit)
    {
        // Input
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        processInput(window);

        // Start the Dear ImGui frame
        gui->drawControlPanel(&program_exit, &culling, &z_buffer, &use_cube_color, &cube_color, &clear_color);
        
        // Rendering
        int display_w, display_h;
        glfwMakeContextCurrent(window);
        glfwGetFramebufferSize(window, &display_w, &display_h);



        glm::mat4 view = camera->GetViewMatrix();
        glm::mat4 proj = glm::perspective(glm::radians(45.0f), (float) SCR_WIDTH / (float) SCR_HEIGHT, 0.1f, 100.0f);
        glm::mat4 model = glm::mat4(1.0f);
        glm::mat4 mvp = proj * view * model;

        glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &mvp[0][0]);
        glViewport(0, 0, display_w, display_h);
        glClearColor(clear_color.x, clear_color.y, clear_color.z, clear_color.w);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glDepthFunc(GL_LESS);
        glUseProgram(shader_programme);

        if(culling)
        {
            glEnable(GL_CULL_FACE);
        }
        else
        {
            glDisable(GL_CULL_FACE);
        }

        if(z_buffer)
        {
            glEnable(GL_DEPTH_TEST);
        }
        else
        {
            glDisable(GL_DEPTH_TEST);
        }

        if(use_cube_color)
        {
            for(int i = 0; i < 36*3; i+=3)
            {
                g_solid_color_buffer_data[i] = cube_color.x;
                g_solid_color_buffer_data[i+1] = cube_color.y;
                g_solid_color_buffer_data[i+2] = cube_color.z;
            }
            glBufferData(GL_ARRAY_BUFFER, sizeof(g_solid_color_buffer_data), g_solid_color_buffer_data, GL_STATIC_DRAW);
            update_color_buffer = false;
        }

        else if(!use_cube_color)
        {
            glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);
            update_color_buffer = true;
        }

        // 1st attribute buffer : vertices
        glEnableVertexAttribArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
        glVertexAttribPointer(
           0,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
           3,                  // size
           GL_FLOAT,           // type
           GL_FALSE,           // normalized?
           0,                  // stride
           nullptr            // array buffer offset
        );
        // Draw the triangle !
        glDrawArrays(GL_TRIANGLES, 0, 12*3); // Starting from vertex 0; 3 vertices total -> 1 triangle

        // 2nd attribute buffer : colors
        glEnableVertexAttribArray(1);
        glBindBuffer(GL_ARRAY_BUFFER, colorbuffer);
        glVertexAttribPointer(
            1,                                // attribute. No particular reason for 1, but must match the layout in the shader.
            3,                                // size
            GL_FLOAT,                         // type
            GL_FALSE,                         // normalized?
            0,                                // stride
            nullptr                          // array buffer offset
        );
        glDisableVertexAttribArray(0);

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
