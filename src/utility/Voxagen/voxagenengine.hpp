#ifndef VOXAGENENGINE_HPP
#define VOXAGENENGINE_HPP

#include "voxagenrenderer.hpp"
#include "gui/guimanager.hpp"
#include "utility/Input/inputcontroller.hpp"
#include "utility/Chunk/chunkmanager.hpp"

#include "utility/ChunkIO/chunkio.hpp"
#include "utility/ChunkIO/filechunkio.hpp"
#include "utility/ChunkIO/gorpchunkio.hpp"

class VoxagenEngine
{
public:

    inline static ChunkManager CHUNK_MANAGER = ChunkManager();
    inline static GuiManager GUI_MANAGER = GuiManager(VoxagenRenderer::WINDOW);
    inline static InputController INPUT_MANAGER = InputController();

    inline static ChunkIO* FILE_IO = new FileChunkIO();
    inline static ChunkIO* GORP_IO = new GorpChunkIO();
};

#endif // VOXAGENENGINE_HPP
