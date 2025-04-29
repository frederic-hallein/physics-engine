#include "PhysicsEngine.hpp"

void framebufferSizeCallback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

PhysicsEngine::PhysicsEngine(
    const char* engineName,
    int screenWidth,
    int screenHeight
)
    : m_screenWidth(screenWidth),
      m_screenHeight(screenHeight)
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

    auto shaderManager = std::make_unique<ShaderManager>();
    auto meshManager = std::make_unique<MeshManager>();
    auto textureManager = std::make_unique<TextureManager>();

    // initialize shaders
    auto dirtBlockShader = std::make_unique<Shader>(
        "dirtBlockShader",
        "../res/shaders/v_shader.txt",
        "../res/shaders/f_shader.txt"
    );

    // initialize meshes
    auto cubeMesh = std::make_unique<Mesh>(
        "cubeMesh",
        "../res/meshes/cube.obj"
    );

    // initialize textures
    auto dirtBlockTexture = std::make_unique<Texture>(
        "dirtBlockTexture",
        "../res/textures/dirt_block.jpg"
    );

    shaderManager->addShader(std::move(dirtBlockShader));
    meshManager->addMesh(std::move(cubeMesh));
    textureManager->addTexture(std::move(dirtBlockTexture));

    float FOV = 45.0f;
    float nearPlane = 0.1f;
    float farPlane = 100.0f;
    float aspectRatio = (float)m_screenWidth / (float)m_screenHeight;
    auto camera = std::make_unique<Camera>(
        glm::vec3(0.0f, 0.0f,  3.0f),
        glm::vec3(0.0f, 0.0f, -1.0f),
        glm::vec3(0.0f, 1.0f,  0.0f),
        FOV,
        aspectRatio,
        nearPlane,
        farPlane,
        m_window
    );

    m_scene = std::make_unique<Scene>(
        "Test Scene",
        std::move(shaderManager),
        std::move(meshManager),
        std::move(textureManager),
        std::move(camera)
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
    const int targetFPS = 60;
    const int targetFrameTime = 1000 / targetFPS;

    while (!glfwWindowShouldClose(m_window))
    {
        auto frameStart = std::chrono::high_resolution_clock::now();

        // Per-frame time logic
        float currentFrame = static_cast<float>(glfwGetTime());
        m_deltaTime = currentFrame - m_lastFrame;
        m_lastFrame = currentFrame;

        processInput(m_window);
        m_scene->render(m_deltaTime);

        glfwSwapBuffers(m_window);
        glfwPollEvents();

        auto frameEnd = std::chrono::high_resolution_clock::now();
        auto frameDuration = std::chrono::duration_cast<std::chrono::milliseconds>(frameEnd - frameStart);

        // Sleep to cap the frame rate
        if (frameDuration.count() < targetFrameTime)
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(targetFrameTime - frameDuration.count()));
        }

        std::cout << "Frame Time: " << frameDuration.count() << " ms" << '\n';
    }
}

void PhysicsEngine::close()
{
    m_scene->clear();
    glfwTerminate();
    std::cout << "PhysicsEngine closed.\n";
}