#ifndef VOXAGENENGINE_HPP
#define VOXAGENENGINE_HPP

#include "voxagenrenderer.hpp"
#include "gui/guimanager.hpp"
#include "inputcontroller.hpp"
#include "chunkmanager.hpp"

class VoxagenEngine
{
public:

    inline static ChunkManager CHUNK_MANAGER = ChunkManager();
    inline static GuiManager GUI_MANAGER = GuiManager(VoxagenRenderer::WINDOW);
    inline static InputController INPUT_MANAGER = InputController();
};

#endif // VOXAGENENGINE_HPP
