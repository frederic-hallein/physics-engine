#pragma once

#include <iostream>
#include <vector>
#include <memory>
#include <glad.h>
#include <GLFW/glfw3.h>

#include "ShaderManager.hpp"
#include "MeshManager.hpp"
#include "TextureManager.hpp"

#include "Scene.hpp"
#include "Object.hpp"

class PhysicsEngine
{
public:
    PhysicsEngine(const char* engineName, int screenWidth, int screenHeight);

    bool isRunning() const { return m_isRunning; }
    void handleEvents();
    void render();
    void close();

private:
    bool m_isRunning = true;
    int unsigned m_screenWidth;
    int unsigned m_screenHeight;
    GLFWwindow* m_window;

    std::unique_ptr<Scene> m_scene;
};