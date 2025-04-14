#include <iostream>
#include <cmath>
#include <glad.h>
#include <GLFW/glfw3.h>

#include "PhysicsEngine.hpp"
#include "Shader.hpp"
#include "Vector3D.hpp"

void framebufferSizeCallback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

PhysicsEngine::PhysicsEngine(const char* name, int width, int height)
{
    std::cout << "Initialize: " << name << '\n';

    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    m_window = glfwCreateWindow(width, height, "Physics Engine", NULL, NULL);
    if(!m_window)
    {
        glfwTerminate();
        std::cout << "Failed to create GLFW window" << '\n';
    }

    glfwMakeContextCurrent(m_window);
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        glfwTerminate();
        std::cout << "Failed to load GLAD" << '\n';
    }

    // // adjust viewport when window resizing
    // framebufferSizeCallback(m_window, width, height);
    // glfwSetFramebufferSizeCallback(m_window, framebufferSizeCallback);

    std::cout << "GLFW window created.\n";


    myShader = Shader("../shaders/v_shader.txt", "../shaders/f_shader.txt");

    // vertex input
    Vector3D vert1 = Vector3D(-0.5f,-0.5f, 0.0f);
    Vector3D vert2 = Vector3D( 0.0f, 0.5f, 0.0f);
    Vector3D vert3 = Vector3D( 0.5f,-0.5f, 0.0f);

    Vector3D col1 = Vector3D( 1.0f, 0.0f, 0.0f);
    Vector3D col2 = Vector3D( 0.0f, 1.0f, 0.0f);
    Vector3D col3 = Vector3D( 0.0f, 0.0f, 1.0f);

    float vertices[18] = {
        // positions                // color
        vert1.x, vert1.y, vert1.z,  col1.x, col1.y, col1.z,
        vert2.x, vert2.y, vert2.z,  col2.x, col2.y, col2.z,
        vert3.x, vert3.y, vert3.z,  col3.x, col3.y, col3.z
    };


    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
};

void PhysicsEngine::handleEvents()
{
    if (glfwWindowShouldClose(m_window))
    {
        m_isRunning = false;
    }
}

void processInput(GLFWwindow* window)
{
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

void PhysicsEngine::render()
{
    while (!glfwWindowShouldClose(m_window))
    {
        processInput(m_window);

        // rendering
        glClearColor(0.5f, 0.6f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        myShader.useProgram();
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); // wireframe

        // check and call events and swap the buffers
        glfwPollEvents();
        glfwSwapBuffers(m_window);

    }
}

void PhysicsEngine::close()
{
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    myShader.deleteProgram();
    glfwTerminate();
    std::cout << "PhysicsEngine closed.\n";
}