#pragma once

#include <imgui.h>
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>

class ImGuiWindow
{
public:
    ImGuiWindow(GLFWwindow* window);

    void update();
    void render();
    void close();

private:

};