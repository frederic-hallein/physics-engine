#pragma once

#include <vector>
#include <memory>

#include "Shader.hpp"

class PhysicsEngine
{
public:
    PhysicsEngine(const char* name, int width, int height);

    bool isRunning() const { return m_isRunning; }
    void handleEvents();
    void initShaders();
    void render();
    void close();

private:
    bool m_isRunning = true;
    GLFWwindow* m_window;

    std::vector<std::unique_ptr<Shader>> shaders;
    unsigned int VBO, VAO, EBO;
    unsigned int texture;
};