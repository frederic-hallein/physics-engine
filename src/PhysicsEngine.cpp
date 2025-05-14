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
      m_screenHeight(screenHeight),
      m_deltaTime(0.0f),
      m_lastFrame(0.0f)
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

    auto shaderManager = std::make_unique<ShaderManager>();
    auto meshManager = std::make_unique<MeshManager>();
    auto textureManager = std::make_unique<TextureManager>();

    // TODO : load all files present in each folder
    // initialize shaders
    auto dirtBlockShader = std::make_unique<Shader>(
        "dirtblock",
        "../res/shaders/dirtblock.vsh",
        "../res/shaders/dirtblock.fsh"
    );

    auto platformShader = std::make_unique<Shader>(
        "platform",
        "../res/shaders/platform.vsh",
        "../res/shaders/platform.fsh"
    );


    // initialize meshes
    auto cubeMesh = std::make_unique<Mesh>(
        "cube",
        "../res/meshes/cube.obj"
    );

    // initialize textures
    auto dirtBlockTexture = std::make_unique<Texture>(
        "dirtblock",
        "../res/textures/dirtblock.jpg"
    );

    shaderManager->addShader(std::move(dirtBlockShader));
    shaderManager->addShader(std::move(platformShader));
    meshManager->addMesh(std::move(cubeMesh));
    textureManager->addTexture(std::move(dirtBlockTexture));

    float FOV = 45.0f;
    float nearPlane = 0.1f;
    float farPlane = 100.0f;
    float aspectRatio = (float)m_screenWidth / (float)m_screenHeight;
    auto camera = std::make_unique<Camera>(
        glm::vec3(0.0f, 0.5f,  8.0f),
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
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

void PhysicsEngine::render()
{
    const int targetFPS = 120;
    Timer timer;

    while (!glfwWindowShouldClose(m_window))
    {
        timer.startFrame();

        processInput(m_window);
        m_deltaTime = timer.getDeltaTime();
        m_scene->update(m_deltaTime);
        m_scene->render();

        glfwSwapBuffers(m_window);
        glfwPollEvents();

        timer.capFrameRate(targetFPS);
        // timer.printFrameDuration();
    }
}

void PhysicsEngine::close()
{
    m_scene->clear();
    glfwTerminate();
    std::cout << "PhysicsEngine closed.\n";
}