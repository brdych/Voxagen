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

void GuiManager::drawControlPanel(bool* program_exit, ImVec4* clear_color) {
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
    if (show_demo_window) {
        ImGui::ShowDemoWindow(&show_demo_window);
    }
    static float f = 0.0f;
    ImGui::Begin("Voxagen Control Panel");
    ImGui::Text("Some Useful Info");
    ImGui::Checkbox("Demo Window", &show_demo_window);      // Edit bools storing our window open/close state

    ImGui::SliderFloat("float", &f, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
    ImGui::ColorEdit3("clear color", (float*)clear_color);  // Edit 3 floats representing a color

    if (ImGui::Button("Exit Voxagen")) {
        *program_exit = true;
    }

    ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
    ImGui::End();
    ImGui::Render();
}

