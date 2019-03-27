#include <GLFW/glfw3.h>
#include <iostream>
#include <cstdio>
#include <glm/gtc/matrix_transform.hpp>

#include <stdio.h>
#include <execinfo.h>
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>



void handler(int sig) {
  void *array[30];
  size_t size;

  // get void*'s for all entries on the stack
  size = backtrace(array, 30);

  // print out all the frames to stderr
  fprintf(stderr, "Error: signal %d:\n", sig);
  backtrace_symbols_fd(array, size, STDERR_FILENO);
  exit(1);
}

#include "worldvariables.hpp"
#include "utility/Voxagen/voxagenengine.hpp"
#include "utility/Chunk/chunkhelper.hpp"

int main()
{
    //signal(SIGSEGV, handler);
    std::cout << "Voxagen Started!" << std::endl;
    while(!glfwWindowShouldClose(VoxagenEngine::WINDOW)&&!WorldVariables::PROGRAM_SHOULD_EXIT)
    {
        // Input
        float currentFrame = static_cast<float>(glfwGetTime());
        WorldVariables::DELTA_TIME = currentFrame - WorldVariables::LAST_FRAME;
        WorldVariables::LAST_FRAME = currentFrame;

        glClearColor(WorldVariables::CLEAR_COLOUR.x, WorldVariables::CLEAR_COLOUR.y, WorldVariables::CLEAR_COLOUR.z, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glDepthFunc(GL_LESS);

        // Update Position Variables
        WorldVariables::CUR_POS = VoxagenEngine::CAMERA.Position;

        WorldVariables::CUR_POS_INT = glm::ivec3(ChunkHelper::FloatWorldToIntWorld(WorldVariables::CUR_POS.x),
                                                 ChunkHelper::FloatWorldToIntWorld(WorldVariables::CUR_POS.y),
                                                 ChunkHelper::FloatWorldToIntWorld(WorldVariables::CUR_POS.z));

        WorldVariables::CUR_CHUNK = glm::ivec3(ChunkHelper::WorldCoordToChunk(WorldVariables::CUR_POS_INT.x),
                                               ChunkHelper::WorldCoordToChunk(WorldVariables::CUR_POS_INT.y),
                                               ChunkHelper::WorldCoordToChunk(WorldVariables::CUR_POS_INT.z));


        WorldVariables::CUR_POS_CHUNK = glm::ivec3(ChunkHelper::WorldCoordToChunkCoord(WorldVariables::CUR_POS_INT.x),
                                                   ChunkHelper::WorldCoordToChunkCoord(WorldVariables::CUR_POS_INT.y),
                                                   ChunkHelper::WorldCoordToChunkCoord(WorldVariables::CUR_POS_INT.z));

        WorldVariables::LOOKING_AT = WorldVariables::CUR_POS + VoxagenEngine::CAMERA.Front * 8.0f;

        WorldVariables::EDIT_CHUNK = glm::ivec3(ChunkHelper::WorldCoordToChunk(WorldVariables::EDIT_POS.x),
                                                ChunkHelper::WorldCoordToChunk(WorldVariables::EDIT_POS.y),
                                                ChunkHelper::WorldCoordToChunk(WorldVariables::EDIT_POS.z));

        WorldVariables::EDIT_CHUNK_POS = glm::ivec3(ChunkHelper::WorldCoordToChunkCoord(WorldVariables::EDIT_POS.x),
                                                    ChunkHelper::WorldCoordToChunkCoord(WorldVariables::EDIT_POS.y),
                                                    ChunkHelper::WorldCoordToChunkCoord(WorldVariables::EDIT_POS.z));

        if(WorldVariables::ENTERED_EDIT_MODE)
        {
            WorldVariables::EDIT_POS = WorldVariables::LOOKING_AT;
            WorldVariables::ENTERED_EDIT_MODE = false;
        }

        if(WorldVariables::CUR_POS.y < 0 && !WorldVariables::STOP_EFFECTS)
        {
            WorldVariables::CLEAR_COLOUR = glm::vec3(0.204f, 0.268f, 0.279f);
            WorldVariables::GLOBAL_LIGHT_COL = glm::vec3(0.363f, 0.363f, 0.363f);
            WorldVariables::FOG_INFO.z = 0.003f;
        }
        else if(!WorldVariables::STOP_EFFECTS)
        {
            WorldVariables::CLEAR_COLOUR =     glm::vec3(0.09f, 0.09f, 1.0f);
            WorldVariables::GLOBAL_LIGHT_COL = glm::vec3(1,1,1);
        }

        if(!WorldVariables::EDIT_MODE && WorldVariables::GRAVITY)
        {
            glm::vec3 feet = glm::vec3(WorldVariables::CUR_POS_INT.x, WorldVariables::CUR_POS_INT.y-3, WorldVariables::CUR_POS_INT.z);
            int cx = ChunkHelper::WorldCoordToChunk(feet.x);
            int cy = ChunkHelper::WorldCoordToChunk(feet.y);
            int cz = ChunkHelper::WorldCoordToChunk(feet.z);
            Chunk* c = VoxagenEngine::CHUNK_MANAGER.ChunkStore.GetChunkV2(cx,cy,cz);

            int lx = ChunkHelper::WorldCoordToChunkCoord(feet.x);
            int ly = ChunkHelper::WorldCoordToChunkCoord(feet.y);
            int lz = ChunkHelper::WorldCoordToChunkCoord(feet.z);

            if(c->GetBlock(lx,ly,lz)==0)
                VoxagenEngine::CAMERA.Position.y-=1;
        }

        // Update View
        VoxagenEngine::VIEW_MAT = VoxagenEngine::CAMERA.GetViewMatrix();

        // Update Controller
        VoxagenEngine::INPUT_MANAGER.ProcessInput(WorldVariables::DELTA_TIME);

        // Update Chunks
        VoxagenEngine::CHUNK_MANAGER.Update();

        // Render Chunks
        VoxagenEngine::CHUNK_MANAGER.Render(&VoxagenEngine::VIEW_MAT, &VoxagenEngine::PROJ_MAT, &VoxagenEngine::MVP_MAT);

        // Render Dear ImGui
        if(WorldVariables::MENU) { VoxagenEngine::GUI_MANAGER.drawControlPanel(); }

        // Clear RenderList for next frame
        VoxagenEngine::CHUNK_MANAGER.ClearRenderList();

        glfwSwapBuffers(VoxagenEngine::WINDOW);
        glfwPollEvents();
    }
    std::cout << "Shutting Down Voxagen!" << std::endl;
    return 0;
}
