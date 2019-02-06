#include "loader.hpp"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

void FrameBufferSizeCallback(GLFWwindow* window, int width, int height);

Loader::Loader()
{
    //Initialize GLFW
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_SAMPLES, 4);
}

Loader::~Loader()
{
    glfwDestroyWindow(window);
    glfwTerminate();
}

GLFWwindow* Loader::LoadGL(int width, int height)
{
    window = glfwCreateWindow(width, height, "Voxagen", nullptr, nullptr);
    glfwMakeContextCurrent(window);

    if (window == nullptr)
    {
        cout << "Failed to create GLFW window" << endl;
        glfwTerminate();
        return nullptr;
    }

    // Initialize GLAD
    if (!gladLoadGLLoader( (GLADloadproc) glfwGetProcAddress))
    {
        cout << "Failed to initialize GLAD" << endl;
        return nullptr;
    }

    glfwSetFramebufferSizeCallback(window, FrameBufferSizeCallback);

    // Set 1 for vsync
    glfwSwapInterval(0);

    // Set Window Icon
    GLFWimage icons[1];
    icons[0].pixels = stbi_load("../src/textures/voxagen_128.png", &icons[0].width, &icons[0].height, nullptr, 0);
    glfwSetWindowIcon(window, 1, icons);

    return window;
}

void FrameBufferSizeCallback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}




