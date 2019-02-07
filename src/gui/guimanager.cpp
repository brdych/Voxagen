#include "gui/guimanager.hpp"
#include "utility/chunkmanager.hpp"
#include "utility/chunkstorage.hpp"

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

void GuiManager::drawCameraInfoPanel(bool* p_open)
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

void GuiManager::drawChunkInfoPanel(bool* p_open)
{
    ChunkManager* cm = ChunkManager::ChunkManagerInstance();
    if (!ImGui::Begin("Chunk Info", p_open))
    {
        ImGui::End();
        return;
    }
    if (!ImGui::CollapsingHeader("LoadList"))
    {
        ImGui::Text("List Size: %i", cm->ChunkLoadList->size());
        ImGui::Text("Threads: %i",WorldVariables::NUM_GEN_THREADS);
        ImGui::Text("Update Time: %.3f ms", WorldVariables::LOADLIST_TIME/1000);
        ImGui::Checkbox("Load Bounds", &WorldVariables::SHOW_LOAD_BOUNDS);
        ImGui::Separator();
    }
    if (!ImGui::CollapsingHeader("MeshList"))
    {
        ImGui::Text("List Size: %i", cm->ChunkMeshList->size());
        ImGui::Text("Threads: %i",WorldVariables::NUM_MESH_THREADS);
        ImGui::Text("Update Time: %.3f ms", WorldVariables::MESHLIST_TIME/1000);
        ImGui::Checkbox("Mesh Bounds", &WorldVariables::SHOW_MESH_BOUNDS);
        ImGui::Separator();
    }

    if (!ImGui::CollapsingHeader("VisibleList"))
    {
        ImGui::Text("List Size: %i", cm->ChunkVisibleList->size());
        ImGui::Text("Update Time: %.3f ms", WorldVariables::VISIBLELIST_TIME/1000);
        ImGui::Checkbox("Visible Bounds", &WorldVariables::SHOW_VISIBLE_BOUNDS);
        ImGui::Separator();
    }

    if (!ImGui::CollapsingHeader("RenderList"))
    {
        ImGui::Text("List Size: %i", cm->ChunkRenderList->size());
        ImGui::Text("Update Time: %.3f ms", WorldVariables::RENDERLIST_TIME/1000);
        ImGui::Checkbox("Render Bounds", &WorldVariables::SHOW_RENDER_BOUNDS);
        ImGui::Separator();
    }

    if (!ImGui::CollapsingHeader("EmptyList"))
    {
        ImGui::Text("List Size: %i", cm->ChunkEmptyList->size());
        ImGui::Text("Update Time: %.3f ms", WorldVariables::EMPTYLIST_TIME/1000);
        ImGui::Checkbox("Empty Bounds", &WorldVariables::SHOW_EMPTY_BOUNDS);
        ImGui::Separator();
    }


    if (!ImGui::CollapsingHeader("UnloadList"))
    {
        ImGui::Text("List Size: %i", cm->ChunkUnloadList->size());
        ImGui::Text("Update Time: %.3f ms", WorldVariables::UNLOADLIST_TIME/1000);
        ImGui::Checkbox("Unload Bounds", &WorldVariables::SHOW_UNLOAD_BOUNDS);
        ImGui::Separator();
    }

    if (!ImGui::CollapsingHeader("Chunk Storage"))
    {
        ImGui::Text("Stored Chunks: %i (%i MB)", ChunkStorage::NUM_CHUNKS_STORED, (ChunkStorage::NUM_CHUNKS_STORED*sizeof(Chunk))/1000);
        ImGui::Text("Chunk Load Time: %.3f ms", WorldVariables::LOADCHUNKS_TIME/1000);
        ImGui::Separator();
    }

    ImGui::Separator();

    ImGui::Text("View Distance: ");
    ImGui::SliderInt("",&WorldVariables::VIEW_DISTANCE,2,20);

    ImGui::End();
}

void GuiManager::drawControlPanel(Camera* c) {
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
    if (show_demo_window) {
        ImGui::ShowDemoWindow(&show_demo_window);
    }
    if (show_chunk_info) {
        drawChunkInfoPanel(&show_chunk_info);
    }
    if (show_camera_info) {
        drawCameraInfoPanel(&show_camera_info);
    }
    ImGui::Begin("Voxagen Control Panel");
    ImGui::Checkbox("Demo Window",          &show_demo_window);
    ImGui::SameLine();
    ImGui::Checkbox("Chunk Info",          &show_chunk_info);
    ImGui::SameLine();
    ImGui::Checkbox("Camera Info",          &show_camera_info);
    ImGui::Separator();
    ImGui::Checkbox("GL Culling",           &WorldVariables::CULLING_ENABLED);
    ImGui::SameLine();
    ImGui::Checkbox("GL Depth Buffer",      &WorldVariables::Z_BUFFER_ENABLED);
    ImGui::SameLine();
    ImGui::Checkbox("Wireframe",            &WorldVariables::USE_WIREFRAME);
    ImGui::SameLine();
    ImGui::Checkbox("Freeze RenderList",    &WorldVariables::FREEZE_RENDERLIST);
    ImGui::Separator();
    ImGui::ColorEdit3("Clear Color",        (float*)WorldVariables::CLEAR_COLOUR);
    ImGui::ColorEdit3("Global Light Color", (float*)WorldVariables::GLOBAL_LIGHT_COL);
    ImGui::SliderFloat("Fog Density",       &WorldVariables::FOG_INFO->z, 0, 0.05f);
    ImGui::Separator();
    ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
    ImGui::Separator();
    if (ImGui::Button("Update Light Pos")) {
        WorldVariables::LIGHT_POS = new glm::vec3(c->Position);
        WorldVariables::GLOBAL_LIGHT_DIR = new glm::vec3(c->Front);
    }
    ImGui::SameLine();
    if (ImGui::Button("Exit Voxagen")) {
        WorldVariables::PROGRAM_SHOULD_EXIT = true;
    }
    ImGui::SameLine();
    if (ImGui::Button("Hide Panel")) {
        WorldVariables::MENU = false;
    }

    ImGui::End();
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

