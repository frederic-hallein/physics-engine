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
    std::cout << "GLFW window created.\n";

    // adjust viewport when window resizing
    framebufferSizeCallback(m_window, screenWidth, screenHeight);
    glfwSetFramebufferSizeCallback(m_window, framebufferSizeCallback);

    const char* glslVersion = "#version 330";
    m_debugWindow = std::make_unique<DebugWindow>(
        m_window,
        glslVersion
    );
    std::cout << "ImGuiWindow created.\n";

    m_timer = std::make_unique<Timer>();

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

    auto sphereShader = std::make_unique<Shader>(
        "sphere",
        "../res/shaders/sphere.vsh",
        "../res/shaders/sphere.fsh"
    );


    // initialize meshes
    auto cubeMesh = std::make_unique<Mesh>(
        "cube",
        "../res/meshes/cube.obj"
    );

    auto sphereMesh = std::make_unique<Mesh>(
        "sphere",
        "../res/meshes/sphere.obj"
    );

    // initialize textures
    auto dirtBlockTexture = std::make_unique<Texture>(
        "dirtblock",
        "../res/textures/dirtblock.jpg"
    );

    shaderManager->addShader(std::move(dirtBlockShader));
    shaderManager->addShader(std::move(platformShader));
    shaderManager->addShader(std::move(sphereShader));
    meshManager->addMesh(std::move(cubeMesh));
    meshManager->addMesh(std::move(sphereMesh));
    textureManager->addTexture(std::move(dirtBlockTexture));

    float FOV = 45.0f;
    float nearPlane = 0.1f;
    float farPlane = 100.0f;
    float aspectRatio = (float)m_screenWidth / (float)m_screenHeight;
    auto camera = std::make_unique<Camera>(
        glm::vec3(0.0f, 3.0f,  10.0f),
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

void processInput(GLFWwindow* window)
{
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

void PhysicsEngine::render()
{
    const int targetFPS = 60;
    while (!glfwWindowShouldClose(m_window))
    {
        processInput(m_window);

        m_timer->startFrame();

        m_scene->update(m_timer->getDeltaTime());
        m_scene->render();

        m_debugWindow->newFrame();
        m_debugWindow->update(
            m_timer->frameDuration,
            *m_scene
        );
        m_debugWindow->render();

        glfwSwapBuffers(m_window);
        glfwPollEvents();

        m_timer->capFrameRate(targetFPS);
    }
}

void PhysicsEngine::close()
{
    m_debugWindow->close();
    m_scene->clear();
    glfwTerminate();
    std::cout << "PhysicsEngine closed.\n";
}