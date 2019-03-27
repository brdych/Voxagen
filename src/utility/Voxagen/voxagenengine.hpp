#ifndef VOXAGENENGINE_HPP
#define VOXAGENENGINE_HPP

#include <GLFW/glfw3.h>
#include <thread>

#include "glm/common.hpp"
#include "glm/mat4x4.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "utility/Camera/camera.hpp"
#include "loader.hpp"
#include "utility/Shader/shader.hpp"

#include "gui/guimanager.hpp"
#include "utility/Input/inputcontroller.hpp"
#include "utility/Chunk/chunkmanager.hpp"

#include "utility/ChunkIO/chunkio.hpp"
#include "utility/ChunkIO/filechunkio.hpp"
#include "utility/ChunkIO/gorpchunkio.hpp"
#include "utility/3P/ThreadPool-master/ThreadPool.h"

class VoxagenEngine
{
public:
    inline static int SCR_WIDTH = 1920;
    inline static int SCR_HEIGHT = 1080;

    inline static float FOV = glm::radians(50.0f);
    inline static float NEAR_PLANE = 0.1f;
    inline static float FAR_PLANE = 10000.0f;

    inline static bool USE_GORB = false;
    inline static bool DEBUG_LIST = false;

    inline static glm::mat4 PROJ_MAT = glm::perspective(FOV, static_cast<float>(SCR_WIDTH)/static_cast<float>(SCR_HEIGHT), NEAR_PLANE, FAR_PLANE);
    inline static glm::mat4 VIEW_MAT = glm::mat4(0);
    inline static glm::mat4 MVP_MAT = glm::mat4(0);

    inline static Camera CAMERA = Camera(glm::vec3(0.0f, 264.0f, 0.0f));
    inline static Loader LOADER = Loader();
    inline static GLFWwindow* WINDOW = LOADER.LoadGL(SCR_WIDTH, SCR_HEIGHT);

    inline static Shader TERRAIN_SHADER = Shader("../src/shaders/BlockShader.vert","../src/shaders/BlockShader.frag");
    inline static Shader WATER_SHADER = Shader("../src/shaders/WaterShader.vert","../src/shaders/WaterShader.frag");

    inline static ChunkManager CHUNK_MANAGER = ChunkManager();
    inline static GuiManager GUI_MANAGER = GuiManager(WINDOW);
    inline static InputController INPUT_MANAGER = InputController();
    inline static ChunkIO* FILE_IO = new FileChunkIO();
    inline static ChunkIO* GORB_IO = new GorbChunkIO();

    inline static uint THREAD_COUNT = std::thread::hardware_concurrency() > 1 ? std::thread::hardware_concurrency()-1 : 1;
    inline static ThreadPool* POOL = new ThreadPool(THREAD_COUNT);
};
#endif // VOXAGENENGINE_HPP
