#ifndef LOADER_HPP
#define LOADER_HPP

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stdio.h>
#include <iostream>
#include <stdlib.h>

using namespace std;

class Loader
{
public:
    Loader();
    ~Loader();
    GLFWwindow* loadGL(int width, int height);


private:
    GLFWwindow* window;
};

#endif // LOADER_HPP
