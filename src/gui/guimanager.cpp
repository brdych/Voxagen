#include "gui/guimanager.hpp"

GuiManager::GuiManager(GLFWwindow* window) {
    // Setup Dear ImGui binding
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 130");
    // Setup style
    ImGui::StyleColorsDark();
}

GuiManager::GuiManager() {

}

GuiManager::GuiManager(GuiManager& orig) {
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
    if (show_demo_window) {
        ImGui::ShowDemoWindow(&show_demo_window);
    }
    static float f = 0.0f;
    ImGui::Begin("Voxagen Control Panel");
    ImGui::Checkbox("Demo Window", &show_demo_window);      // Edit bools storing our window open/close state
    ImGui::Checkbox("GL Culling", &_settings->CULLING_ENABLED);                 // Edit bools storing GL Culling
    ImGui::Checkbox("GL Depth Buffer", &_settings->Z_BUFFER_ENABLED);           // Edit bools storing GL Z_Buffer
    ImGui::Checkbox("Wireframe", &_settings->USE_WIREFRAME);// Edit Wireframe bool
    ImGui::ColorEdit3("clear color", (float*)_settings->CLEAR_COLOUR);  // Edit 3 floats representing a color

    if (ImGui::Button("Regenerate VoxelData")) {
        _settings->REGEN_VOXELDATA = true;
    }

    if (ImGui::Button("Exit Voxagen")) {
        _settings->PROGRAM_SHOULD_EXIT = true;
    }

    ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
    ImGui::End();
    ImGui::Render();
}

