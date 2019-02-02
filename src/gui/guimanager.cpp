#include "gui/guimanager.hpp"
#include "utility/chunkmanager.hpp"

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

void GuiManager::drawChunkInfoPanel(bool* p_open)
{
    ChunkManager* cm = ChunkManager::ChunkManagerInstance();
    if (!ImGui::Begin("Chunk Info", p_open))
    {
        ImGui::End();
        return;
    }
    ImGui::Text("Chunk Loadlist: %i", cm->ChunkLoadList->size());

    ImGui::Text("Chunk Meshlist: %i", cm->ChunkSetupList->size());

    ImGui::Text("Chunk Visiblelist: %i", cm->ChunkVisibleList->size());

    ImGui::Text("Chunk Renderlist: %i", cm->ChunkRenderList->size());

    ImGui::Text("Chunk Unloadlist: %i", cm->ChunkUnloadList->size());

    ImGui::Separator();
    if (ImGui::Button("DELETE ALL")) {
        WorldVariables::DELETE_ALL = true;
    }

    if (ImGui::Button("REBUILD ALL")) {
        WorldVariables::DELETE_ALL = false;
        WorldVariables::REBUILD_ALL = true;
    }

    ImGui::Separator();

    if (ImGui::Button("Close")) {
        *p_open = false;
    }
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
    ImGui::Begin("Voxagen Control Panel");
    ImGui::Checkbox("Demo Window",          &show_demo_window);
    ImGui::SameLine();
    ImGui::Checkbox("Chunk Info",          &show_chunk_info);
    ImGui::Separator();
    ImGui::Checkbox("GL Culling",           &WorldVariables::CULLING_ENABLED);
    ImGui::SameLine();
    ImGui::Checkbox("GL Depth Buffer",      &WorldVariables::Z_BUFFER_ENABLED);
    ImGui::SameLine();
    ImGui::Checkbox("Wireframe",            &WorldVariables::USE_WIREFRAME);
    ImGui::Separator();
    ImGui::Checkbox("Show Chunk Bounds",        &WorldVariables::SHOW_CHUNK_BOUNDS);
    ImGui::Separator();
    ImGui::ColorEdit3("Clear Color",        (float*)WorldVariables::CLEAR_COLOUR);
    ImGui::ColorEdit3("Global Light Color", (float*)WorldVariables::GLOBAL_LIGHT_COL);
    ImGui::SliderFloat("Fog Density",       &WorldVariables::FOG_INFO->z, 0, 0.05f);
    ImGui::Separator();
    ImGui::Text("Camera Location: x:%.3f y:%.3f z:%.3f",c->Position.x,c->Position.y,c->Position.z);
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

