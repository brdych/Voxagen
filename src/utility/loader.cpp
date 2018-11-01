#include "loader.hpp"

Loader::Loader()
{
    //Initialize GLFW
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
}

Loader::~Loader()
{
    glfwDestroyWindow(window);
    glfwTerminate();
}

GLFWwindow* Loader::loadGL(int width, int height)
{
    window = glfwCreateWindow(width, height, "Voxagen", nullptr, nullptr);
    if (window == nullptr)
    {
        cout << "Failed to create GLFW window" << endl;
        glfwTerminate();
        return nullptr;
    }
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);
    // Initialize GLAD
    if (!gladLoadGLLoader( (GLADloadproc) glfwGetProcAddress))
    {
        cout << "Failed to initialize GLAD" << endl;
        return nullptr;
    }
    return window;
}


