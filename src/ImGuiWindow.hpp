#pragma once

#include <iostream>
#include <glm/glm.hpp>
#include <memory>

#include "imgui.h"
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"

#include "Object.hpp"

class ImGuiWindow
{
public:
    ImGuiWindow() = default;
    ImGuiWindow(
        GLFWwindow* window,
        const char* glslVersion
    );

    void newFrame();
    void update();
    void render();
    void close();

private:

};

class DebugWindow : public ImGuiWindow
{
public:
    DebugWindow() = default;
    DebugWindow(
        GLFWwindow* window,
        const char* glslVersion
    );

    void update(
        int frameDuration,
        const glm::vec3& cameraPosition,
        const std::vector<std::unique_ptr<Object>>& objects
    );
};