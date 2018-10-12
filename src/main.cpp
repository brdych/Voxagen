#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <stdio.h>



using namespace std;


#include "world/chunk.h"

int main(void) {

    cout << "Hello world!" << endl;

    static const int WORLD_SIZE = 1;
    Chunk*** _chunks;

    _chunks = new Chunk**[WORLD_SIZE];
    for (int i = 0; i < WORLD_SIZE; i++) {
        _chunks[i] = new Chunk*[WORLD_SIZE];
        for (int j = 0; j < WORLD_SIZE; j++) {
            _chunks[i][j] = new Chunk[WORLD_SIZE];
        }
    }

    //Initialize GLFW
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(800, 600, "Voxagen", NULL, NULL);
    if (window == NULL) {
        cout << "Failed to create GLFW window" << endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);

    // Initialize GLAD
    if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
        cout << "Failed to initialize GLAD" << endl;
        return -1;
    }


    glViewport(0, 0, 800, 600);

    while(!glfwWindowShouldClose(window)) {
        
        // Input
        
        // Rendering
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        
        // Check events/swap the buffers
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    
    glfwTerminate();
    
    cout << "Goodbye world!" << endl;

    return 0;
}
