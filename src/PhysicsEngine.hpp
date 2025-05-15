#pragma once

#include <iostream>
#include <thread>
#include <vector>
#include <memory>
#include <glad.h>
#include <GLFW/glfw3.h>

#include "ImGuiWindow.hpp"
#include "Scene.hpp"
#include "Timer.hpp"

class PhysicsEngine
{
public:
    PhysicsEngine(
        const char* engineName,
        int screenWidth,
        int screenHeight
    );

    bool isRunning() const { return m_isRunning; }
    void handleEvents();
    void render();
    void close();

private:
    bool m_isRunning = true;
    int unsigned m_screenWidth;
    int unsigned m_screenHeight;
    GLFWwindow* m_window;

    float m_deltaTime;
    float m_lastFrame;

    std::unique_ptr<ImGuiWindow> m_imGuiWindow;
    std::unique_ptr<Scene> m_scene;
};