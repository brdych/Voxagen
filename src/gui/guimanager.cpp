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

