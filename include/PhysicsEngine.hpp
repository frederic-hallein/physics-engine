#pragma once

// #include <memory>

#include "Shader.hpp"

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
    Shader myShader;
    unsigned int VBO, VAO;
};