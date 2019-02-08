#ifndef VOXAGENRENDERER_HPP
#define VOXAGENRENDERER_HPP

#include <GLFW/glfw3.h>

#include "glm/common.hpp"
#include "glm/mat4x4.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "camera.hpp"
#include "loader.hpp"

class VoxagenRenderer
{
public:
    inline static int SCR_WIDTH = 1920;
    inline static int SCR_HEIGHT = 1080;

    inline static float FOV = glm::radians(50.0f);
    inline static float NEAR_PLANE = 0.1f;
    inline static float FAR_PLANE = 10000.0f;

    inline static glm::mat4 PROJ_MAT = glm::perspective(FOV, static_cast<float>(SCR_WIDTH)/static_cast<float>(SCR_HEIGHT), NEAR_PLANE, FAR_PLANE);
    inline static glm::mat4 VIEW_MAT = glm::mat4(0);
    inline static glm::mat4 MVP_MAT = glm::mat4(0);

    inline static Camera CAMERA = Camera(glm::vec3(0.0f, 100.0f, 0.0f));
    inline static Loader LOADER = Loader();
    inline static GLFWwindow* WINDOW = LOADER.LoadGL(SCR_WIDTH, SCR_HEIGHT);

};

#endif // VOXAGENRENDERER_HPP
