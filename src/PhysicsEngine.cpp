#include <iostream>
#include <glad.h>
#include <GLFW/glfw3.h>
#include <stb_image.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "PhysicsEngine.hpp"
#include "ShaderManager.hpp"

// camera
glm::vec3 cameraPos   = glm::vec3(0.0f, 0.0f,  3.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp    = glm::vec3(0.0f, 1.0f,  0.0f);

float fov   =  45.0f;

// timing
float deltaTime = 0.0f;	// time between current frame and last frame
float lastFrame = 0.0f;

void framebufferSizeCallback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void scrollCallback(GLFWwindow* window, double xoffset, double yoffset)
{
    float scrollSpeed = static_cast<float>(150.0 * deltaTime);
    cameraPos += cameraFront * static_cast<float>(yoffset) * scrollSpeed;
}

PhysicsEngine::PhysicsEngine(const char* name, int width, int height)
{
    std::cout << "Initialize: " << name << '\n';

    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    m_window = glfwCreateWindow(width, height, "Physics Engine", NULL, NULL);
    if(m_window == NULL)
    {
        glfwTerminate();
        std::cout << "Failed to create GLFW window" << '\n';
    }

    glfwMakeContextCurrent(m_window);
    glfwSetScrollCallback(m_window, scrollCallback);


    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        glfwTerminate();
        std::cout << "Failed to load GLAD" << '\n';
    }

    // // adjust viewport when window resizing
    // framebufferSizeCallback(m_window, width, height);
    // glfwSetFramebufferSizeCallback(m_window, framebufferSizeCallback);

    std::cout << "GLFW window created.\n";


    glEnable(GL_DEPTH_TEST);

    shaderManager = std::make_unique<ShaderManager>();
    shaderManager->addShader("basic", "../res/shaders/v_shader.txt", "../res/shaders/f_shader.txt");

    meshManager = std::make_unique<MeshManager>();
    meshManager->addMesh("cube", "../res/meshes/cube.txt");

    textureManager = std::make_unique<TextureManager>();
    textureManager->addTexture("dirt block", "../res/textures/dirt_block.jpg");
};

void PhysicsEngine::handleEvents()
{
    if (glfwWindowShouldClose(m_window))
    {
        m_isRunning = false;
    }
}

static void processInput(GLFWwindow* window)
{
    float cameraSpeed = static_cast<float>(100 * deltaTime);
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;

}

void PhysicsEngine::render()
{

    while (!glfwWindowShouldClose(m_window))
    {

        // per-frame time logic
        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        processInput(m_window);

        // rendering
        glClearColor(0.2f, 0.2f, 0.8f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // bind textures
        textureManager->bindTexture("dirt block");

        // activate shader
        shaderManager->useShader("basic");

        // projection transformation
        const unsigned int SCR_WIDTH = 1080;
        const unsigned int SCR_HEIGHT = 720;
        glm::mat4 projection = glm::perspective(glm::radians(fov), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        int projectionLoc = glGetUniformLocation(shaderManager->getShader("basic")->ID, "projection");
        glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));

        // model transformation
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::rotate(model, (float)glfwGetTime() * glm::radians(180.0f), glm::vec3(2.0f, 1.0f, 0.5f));
        int modelLoc = glGetUniformLocation(shaderManager->getShader("basic")->ID, "model");
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

        // camera/view transformation
        glm::mat4 view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
        int viewLoc = glGetUniformLocation(shaderManager->getShader("basic")->ID, "view");
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));

        // rendering
        meshManager->drawMesh("cube");

        // // wireframe
        // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

        // swap buffers and poll IO events
        glfwSwapBuffers(m_window);
        glfwPollEvents();

    }
}

void PhysicsEngine::close()
{
    textureManager->deleteAllTextures();
    meshManager->deleteAllMeshes();
    shaderManager->deleteAllShaders();
    glfwTerminate();
    std::cout << "PhysicsEngine closed.\n";
}