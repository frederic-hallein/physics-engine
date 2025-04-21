#pragma once

#include <vector>
#include <memory>

#include "ShaderManager.hpp"
#include "MeshManager.hpp"

class PhysicsEngine
{
public:
    PhysicsEngine(const char* name, int width, int height);

    bool isRunning() const { return m_isRunning; }
    void handleEvents();
    void render();
    void close();

private:
    bool m_isRunning = true;
    GLFWwindow* m_window;

    std::unique_ptr<ShaderManager> shaderManager;
    std::unique_ptr<MeshManager> meshManager;
    unsigned int texture;
};