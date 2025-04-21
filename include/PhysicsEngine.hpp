#pragma once

#include <vector>
#include <memory>

#include "ShaderManager.hpp"
#include "MeshManager.hpp"
#include "TextureManager.hpp"

class PhysicsEngine
{
public:
    PhysicsEngine(const char* name, int screenWidth, int screenHeight);

    bool isRunning() const { return m_isRunning; }
    void handleEvents();
    void render();
    void close();

private:
    bool m_isRunning = true;
    int unsigned m_screenWidth;
    int unsigned m_screenHeight;
    GLFWwindow* m_window;

    std::unique_ptr<ShaderManager> shaderManager;
    std::unique_ptr<MeshManager> meshManager;
    std::unique_ptr<TextureManager> textureManager;
};