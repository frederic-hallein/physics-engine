#include "ImGuiWindow.hpp"

ImGuiWindow::ImGuiWindow(GLFWwindow* window)
{
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    (void)io;

    // Set up ImGui backends
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 330"); // Use your OpenGL version
    ImGui::StyleColorsDark();
}

void ImGuiWindow::update()
{

}

void ImGuiWindow::render()
{
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    float someFloatValue = 0.5f;
    ImGui::Begin("Debug");
    ImGui::Text("Hello, ImGui!");
    ImGui::SliderFloat("Float Slider", &someFloatValue, 0.0f, 1.0f);
    ImGui::End();

    // Render ImGui
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void ImGuiWindow::close()
{
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}