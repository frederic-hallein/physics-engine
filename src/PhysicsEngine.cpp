// #include <iostream>
// #include <glad.h>
// #include <GLFW/glfw3.h>
// #include <stb_image.h>
// #include <glm/glm.hpp>
// #include <glm/gtc/matrix_transform.hpp>
// #include <glm/gtc/type_ptr.hpp>

#include "PhysicsEngine.hpp"


// // camera
// glm::vec3 cameraPos   = glm::vec3(0.0f, 0.0f,  3.0f);
// glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
// glm::vec3 cameraUp    = glm::vec3(0.0f, 1.0f,  0.0f);

// float fov   =  45.0f;

// // timing
// float deltaTime = 0.0f;	// time between current frame and last frame
// float lastFrame = 0.0f;

void framebufferSizeCallback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

// void scrollCallback(GLFWwindow* window, double xoffset, double yoffset)
// {
//     float scrollSpeed = static_cast<float>(150.0 * deltaTime);
//     cameraPos += cameraFront * static_cast<float>(yoffset) * scrollSpeed;
// }

PhysicsEngine::PhysicsEngine(const char* engineName, int screenWidth, int screenHeight)
    : m_screenWidth(screenWidth), m_screenHeight(screenHeight)
{
    std::cout << "Initialize: " << engineName << '\n';

    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    m_window = glfwCreateWindow(screenWidth, screenHeight, engineName, NULL, NULL);
    if(m_window == NULL)
    {
        glfwTerminate();
        std::cout << "Failed to create GLFW window" << '\n';
    }

    glfwMakeContextCurrent(m_window);
    // glfwSetScrollCallback(m_window, scrollCallback);


    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        glfwTerminate();
        std::cout << "Failed to load GLAD" << '\n';
    }

    // adjust viewport when window resizing
    framebufferSizeCallback(m_window, screenWidth, screenHeight);
    glfwSetFramebufferSizeCallback(m_window, framebufferSizeCallback);

    std::cout << "GLFW window created.\n";


    glEnable(GL_DEPTH_TEST);

    auto m_shaderManager = std::make_unique<ShaderManager>();
    auto m_meshManager = std::make_unique<MeshManager>();
    auto m_textureManager = std::make_unique<TextureManager>();

    // initialize shaders
    auto dirtBlockShader = std::make_unique<Shader>("dirtBlockShader", "../res/shaders/v_shader.txt", "../res/shaders/f_shader.txt");

    // initialize meshes
    auto cubeMesh = std::make_unique<Mesh>("cubeMesh", "../res/meshes/cube.txt");

    // initialize textures
    auto dirtBlockTexture = std::make_unique<Texture>("dirtBlockTexture", "../res/textures/dirt_block.jpg");

    m_shaderManager->addShader(std::move(dirtBlockShader));
    m_meshManager->addMesh(std::move(cubeMesh));
    m_textureManager->addTexture(std::move(dirtBlockTexture));

    m_scene = std::make_unique<Scene>(
        "Test Scene",
        std::move(m_shaderManager),
        std::move(m_meshManager),
        std::move(m_textureManager)
    );



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
    // float cameraSpeed = static_cast<float>(100 * deltaTime);
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    // if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    //     cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
    // if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    //     cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;

}

void PhysicsEngine::render()
{

    while (!glfwWindowShouldClose(m_window))
    {

        // // per-frame time logic
        // float currentFrame = static_cast<float>(glfwGetTime());
        // deltaTime = currentFrame - lastFrame;
        // lastFrame = currentFrame;

        processInput(m_window);


        m_scene->render();


        // // projection transformation
        // glm::mat4 projection = glm::perspective(glm::radians(fov), (float)m_screenWidth / (float)m_screenHeight, 0.1f, 100.0f);
        // int projectionLoc = glGetUniformLocation(shaderManager->getShader("basic")->ID, "projection");
        // glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));

        // // model transformation
        // glm::mat4 model = glm::mat4(1.0f);
        // model = glm::rotate(model, (float)glfwGetTime() * glm::radians(180.0f), glm::vec3(2.0f, 1.0f, 0.5f));
        // int modelLoc = glGetUniformLocation(shaderManager->getShader("basic")->ID, "model");
        // glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

        // // camera/view transformation
        // glm::mat4 view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
        // int viewLoc = glGetUniformLocation(shaderManager->getShader("basic")->ID, "view");
        // glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));







        // // TODO : add shortcut + use specific colors
        // // wireframe
        // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

        // swap buffers and poll IO events
        glfwSwapBuffers(m_window);
        glfwPollEvents();

    }
}

void PhysicsEngine::close()
{
    m_scene->clear();
    glfwTerminate();
    std::cout << "PhysicsEngine closed.\n";
}