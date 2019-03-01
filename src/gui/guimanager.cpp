#include "gui/guimanager.hpp"
#include "utility/Voxagen/voxagenengine.hpp"

GuiManager::GuiManager(GLFWwindow* window) {
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 130");
    ImGui::StyleColorsDark();
}

GuiManager::GuiManager() {

}

GuiManager::~GuiManager() {
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}

void GuiManager::drawControlPanel() {
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    ImGui::Begin("Voxagen Engine v0.1");

    if (ImGui::CollapsingHeader("Debug Panels"))
    {
        ImGui::Checkbox("Demo Window",          &show_demo_window);
        ImGui::Checkbox("Chunk Panel",          &show_chunk_panel);
        ImGui::Checkbox("Camera Panel",         &show_camera_panel);
        ImGui::Checkbox("World Panel",          &show_world_panel);
        ImGui::Checkbox("Edit Panel (WIP)",     &show_edit_panel);
        ImGui::Checkbox("GL Panel",             &show_gl_panel);
        ImGui::Checkbox("ChunkIO Panel",        &show_chunkio_panel);
    }

    ImGui::Separator();
    ImGui::Text("Voxagen: %.3f ms/frame (%.1f FPS)", static_cast<double>(1000) / static_cast<double>(ImGui::GetIO().Framerate), static_cast<double>(ImGui::GetIO().Framerate));
    ImGui::Separator();

    if (ImGui::Button("Exit Voxagen")) {
        WorldVariables::PROGRAM_SHOULD_EXIT = true;
    }
    ImGui::SameLine();
    if (ImGui::Button("Hide Panel")) {
        WorldVariables::MENU = false;
    }

    ImGui::End();

    if (show_demo_window) {
        ImGui::ShowDemoWindow(&show_demo_window);
    }
    if (show_chunk_panel) {
        drawChunkPanel(&show_chunk_panel);
    }
    if (show_camera_panel) {
        drawCameraPanel(&show_camera_panel);
    }
    if (show_edit_panel) {
        drawEditPanel(&show_edit_panel);
    }
    if (show_chunkio_panel) {
        drawChunkIOPanel(&show_chunkio_panel);
    }
    if (show_gl_panel) {
        drawGLPanel(&show_gl_panel);
    }
    if (show_world_panel) {
        drawWorldPanel(&show_world_panel);
    }

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void GuiManager::drawEditPanel(bool *p_open)
{
    if (!ImGui::Begin("Edit Panel", p_open))
    {
        ImGui::End();
        return;
    }

    ImGui::Checkbox("Edit Mode", &WorldVariables::EDIT_MODE);
    ImGui::Separator();

    ImGui::End();
}

void GuiManager::drawChunkIOPanel(bool *p_open)
{
    if (!ImGui::Begin("ChunkIO", p_open))
    {
        ImGui::End();
        return;
    }

    if (ImGui::Button("Load Chunks From File")) {
        VoxagenEngine::CHUNK_MANAGER.Reset();
        WorldVariables::LOAD_CHUNKS_FROM_FILE = true;
    }

    if (ImGui::Button("Save Visible Chunks")) {
        WorldVariables::FREEZE_RENDERLIST = true;
        WorldVariables::SAVE_CHUNK_STORE = true;
        WorldVariables::USE_GORP = false;
    }

    if (ImGui::Button("Gorp Visible Chunks")) {
        WorldVariables::FREEZE_RENDERLIST = true;
        WorldVariables::SAVE_CHUNK_STORE = true;
        WorldVariables::USE_GORP = true;
    }

    ImGui::End();
}

void GuiManager::drawWorldPanel(bool *p_open)
{
    if (!ImGui::Begin("World Panel", p_open))
    {
        ImGui::End();
        return;
    }

    ImGui::ColorEdit3("Clear Color",        (float*)&WorldVariables::CLEAR_COLOUR);
    ImGui::ColorEdit3("Global Light Color", (float*)&WorldVariables::GLOBAL_LIGHT_COL);
    ImGui::SliderFloat("Fog Density",       &WorldVariables::FOG_INFO.z, 0, 0.05f);

    ImGui::Separator();
    if (ImGui::Button("Update Light Position"))
    {
        WorldVariables::LIGHT_POS = glm::vec3(VoxagenRenderer::CAMERA.Position);
        WorldVariables::GLOBAL_LIGHT_DIR = glm::vec3(VoxagenRenderer::CAMERA.Front);
    }

    ImGui::End();
}

void GuiManager::drawGLPanel(bool *p_open)
{
    if (!ImGui::Begin("GL Panel", p_open))
    {
        ImGui::End();
        return;
    }

    ImGui::Checkbox("GL Culling",           &WorldVariables::CULLING_ENABLED);
    ImGui::Checkbox("GL Depth Buffer",      &WorldVariables::Z_BUFFER_ENABLED);
    ImGui::Checkbox("GL Wireframe",         &WorldVariables::USE_WIREFRAME);

    ImGui::End();
}

void GuiManager::drawCameraPanel(bool* p_open)
{
    glm::vec3 cPos = WorldVariables::CUR_POS;
    glm::vec3 cPosInt = WorldVariables::CUR_POS_INT;
    glm::vec3 cChunk = WorldVariables::CUR_CHUNK;
    glm::vec3 cChunkPos = WorldVariables::CUR_POS_CHUNK;
    if (!ImGui::Begin("Camera Info", p_open))
    {
        ImGui::End();
        return;
    }
    ImGui::Text("Position: x:%.3f y:%.3f z:%.3f",cPos.x, cPos.y, cPos.z);
    ImGui::Separator();
    ImGui::Text("Position(Integer): x:%.0f y:%.0f z:%.0f",cPosInt.x,cPosInt.y,cPosInt.z);
    ImGui::Separator();
    ImGui::Text("Chunk: x:%.0f y:%.0f z:%.0f",cChunk.x,cChunk.y,cChunk.z);
    ImGui::Separator();
    ImGui::Text("Chunk Pos: x:%.0f y:%.0f z:%.0f",cChunkPos.x,cChunkPos.y,cChunkPos.z);
    ImGui::Separator();
    ImGui::End();
}

void GuiManager::drawChunkPanel(bool* p_open)
{
    if (!ImGui::Begin("Chunk Info", p_open))
    {
        ImGui::End();
        return;
    }
    if (!ImGui::CollapsingHeader("LoadList"))
    {
        ImGui::Text("List Size: %i", static_cast<int>(VoxagenEngine::CHUNK_MANAGER.ChunkLoadList.size()));
        ImGui::Text("Update Time: %.3f ms", static_cast<double>(WorldVariables::LOADLIST_TIME/1000));
        ImGui::Checkbox("Load Bounds", &WorldVariables::SHOW_LOAD_BOUNDS);
        ImGui::Separator();
    }
    if (!ImGui::CollapsingHeader("MeshList"))
    {
        ImGui::Text("List Size: %i", static_cast<int>(VoxagenEngine::CHUNK_MANAGER.ChunkMeshList.size()));
        ImGui::Text("Update Time: %.3f ms", static_cast<double>(WorldVariables::MESHLIST_TIME/1000));
        ImGui::Checkbox("Mesh Bounds", &WorldVariables::SHOW_MESH_BOUNDS);
        ImGui::Separator();
    }

    if (!ImGui::CollapsingHeader("VisibleList"))
    {
        ImGui::Text("List Size: %i", static_cast<int>(VoxagenEngine::CHUNK_MANAGER.ChunkVisibleList.size()));
        ImGui::Text("Update Time: %.3f ms", static_cast<double>(WorldVariables::VISIBLELIST_TIME/1000));
        ImGui::Checkbox("Visible Bounds", &WorldVariables::SHOW_VISIBLE_BOUNDS);
        ImGui::Separator();
    }

    if (!ImGui::CollapsingHeader("RenderList"))
    {
        ImGui::Text("List Size: %i", static_cast<int>(VoxagenEngine::CHUNK_MANAGER.ChunkRenderList.size()));
        ImGui::Text("Update Time: %.3f ms", static_cast<double>(WorldVariables::RENDERLIST_TIME/1000));
        ImGui::Checkbox("Render Bounds", &WorldVariables::SHOW_RENDER_BOUNDS);
        ImGui::Separator();
    }

    if (!ImGui::CollapsingHeader("EmptyList"))
    {
        ImGui::Text("List Size: %i", static_cast<int>(VoxagenEngine::CHUNK_MANAGER.ChunkEmptyList.size()));
        ImGui::Text("Update Time: %.3f ms", static_cast<double>(WorldVariables::EMPTYLIST_TIME/1000));
        ImGui::Checkbox("Empty Bounds", &WorldVariables::SHOW_EMPTY_BOUNDS);
        ImGui::Separator();
    }


    if (!ImGui::CollapsingHeader("UnloadList"))
    {
        ImGui::Text("List Size: %i", static_cast<int>(VoxagenEngine::CHUNK_MANAGER.ChunkUnloadList.size()));
        ImGui::Text("Update Time: %.3f ms", static_cast<double>(WorldVariables::UNLOADLIST_TIME/1000));
        ImGui::Checkbox("Unload Bounds", &WorldVariables::SHOW_UNLOAD_BOUNDS);
        ImGui::Separator();
    }

    if (!ImGui::CollapsingHeader("Chunk Storage"))
    {
        ImGui::Text("Stored Chunks: %i (%.3f MB)", ChunkStorage::NUM_CHUNKS_STORED, ((static_cast<double>(WorldVariables::CHUNKSTORE_STORED_BYTES))/1000)/1000);
        ImGui::Text("Chunk Load Time: %.3f ms", static_cast<double>(WorldVariables::LOADCHUNKS_TIME/1000));
        ImGui::Separator();
    }

    ImGui::Checkbox("Freeze",    &WorldVariables::FREEZE_RENDERLIST);
    ImGui::Separator();

    ImGui::Text("Threads: %i", VoxagenEngine::CHUNK_MANAGER.CurrentThreads);
    ImGui::Separator();

    ImGui::Text("View Distance");
    ImGui::SliderInt("", &view_distance,2,20);
    if(ImGui::Button("Update View Distance"))
    {
        WorldVariables::VIEW_DISTANCE = view_distance;
        VoxagenEngine::CHUNK_MANAGER.Reset();
    }
    ImGui::End();
}

