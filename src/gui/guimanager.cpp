#include "gui/guimanager.hpp"

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

void GuiManager::drawControlPanel(Camera* c) {
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
    if (show_demo_window) {
        ImGui::ShowDemoWindow(&show_demo_window);
    }
    ImGui::Begin("Voxagen Control Panel");
    ImGui::Checkbox("Demo Window",          &show_demo_window);
    ImGui::Separator();
    ImGui::Checkbox("GL Culling",           &_settings->CULLING_ENABLED);
    ImGui::SameLine();
    ImGui::Checkbox("GL Depth Buffer",      &_settings->Z_BUFFER_ENABLED);
    ImGui::SameLine();
    ImGui::Checkbox("Wireframe",            &_settings->USE_WIREFRAME);
    ImGui::Separator();
    ImGui::Checkbox("Show All Chunk Bounds",        &_settings->SHOW_CHUNK_BOUNDS);
    ImGui::Checkbox("Show Rendered Chunk Bounds",   &_settings->ONLY_RENDERING_CHUNKS);
    ImGui::Separator();
    ImGui::ColorEdit3("Clear Color",        (float*)_settings->CLEAR_COLOUR);
    ImGui::ColorEdit3("Global Light Color", (float*)_settings->GLOBAL_LIGHT_COL);
    ImGui::SliderFloat("Fog Density",       &_settings->FOG_INFO->z, 0, 0.05f);
    ImGui::Separator();
    ImGui::Text("Camera Location: x:%.3f y:%.3f z:%.3f",c->Position.x,c->Position.y,c->Position.z);
    ImGui::Separator();
    ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
    ImGui::Separator();
    if (ImGui::Button("Update Light Pos")) {
        _settings->LIGHT_POS = new glm::vec3(c->Position);
        _settings->GLOBAL_LIGHT_DIR = new glm::vec3(c->Front);
    }
    ImGui::SameLine();
    if (ImGui::Button("Exit Voxagen")) {
        _settings->PROGRAM_SHOULD_EXIT = true;
    }
    ImGui::SameLine();
    if (ImGui::Button("Hide Panel")) {
        _settings->MENU = false;
    }

    ImGui::End();
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

