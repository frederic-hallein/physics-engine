#include <iostream>
#include <glad.h>
#include <GLFW/glfw3.h>
// #include <SDL2/SDL.h>
// #include <SDL2/SDL_image.h>

// #include "PhysicsEngine.hpp"

int SCREEN_WIDTH = 1080;
int SCREEN_HEIGHT = 720;

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window)
{
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

int main(int argc, char* argv[])
{
    // PhysicsEngine physicsEngine("Physics Engine", SCREEN_WIDTH, SCREEN_HEIGHT);
    // while(physicsEngine.running())
    // {
    //     physicsEngine.handleEvents();
    //     physicsEngine.render();
    // }

    // physicsEngine.close();

    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Physics Engine", NULL, NULL);
    if (!window)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);


    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        glfwTerminate();
        return -1;
    }

    framebuffer_size_callback(window, SCREEN_WIDTH, SCREEN_HEIGHT);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback); // adjust viewport when window resizing

    while (!glfwWindowShouldClose(window))
    {
        // input
        processInput(window);

        // rendering
        glClearColor(1.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // check and call events and swap the buffers
        glfwPollEvents();
        glfwSwapBuffers(window);

    }

    glfwTerminate();
    return 0;
}
