#ifndef LOADER_HPP
#define LOADER_HPP

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stdio.h>
#include <iostream>
#include <stdlib.h>

class Loader
{
public:
    Loader();
    ~Loader();
    GLFWwindow* LoadGL(int width, int height);

private:
    GLFWwindow* window;
};

#endif // LOADER_HPP
