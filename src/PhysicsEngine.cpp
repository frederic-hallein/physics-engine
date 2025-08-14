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

    // init GLFW window
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


    // create timer
    m_timer = std::make_unique<Timer>();


    // adjust viewport when window resizing
    framebufferSizeCallback(m_window, screenWidth, screenHeight);
    glfwSetFramebufferSizeCallback(m_window, framebufferSizeCallback);


    // adding shaders
    auto shaderManager = std::make_unique<ShaderManager>();
    std::vector<std::unique_ptr<Shader>> shaders;
    shaders.push_back(std::make_unique<Shader>(
        "vertexNormal",
        "../res/shaders/vertexNormal.vsh",
        "../res/shaders/vertexNormal.fsh"
    ));
    shaders.push_back(std::make_unique<Shader>(
        "faceNormal",
        "../res/shaders/faceNormal.vsh",
        "../res/shaders/faceNormal.fsh"
    ));
    shaders.push_back(std::make_unique<Shader>(
        "light",
        "../res/shaders/light.vsh",
        "../res/shaders/light.fsh"
    ));
    shaders.push_back(std::make_unique<Shader>(
        "platform",
        "../res/shaders/platform.vsh",
        "../res/shaders/platform.fsh"
    ));
    shaders.push_back(std::make_unique<Shader>(
        "dirtblock",
        "../res/shaders/dirtblock.vsh",
        "../res/shaders/dirtblock.fsh"
    ));
    shaders.push_back(std::make_unique<Shader>(
        "sphere",
        "../res/shaders/sphere.vsh",
        "../res/shaders/sphere.fsh"
    ));
    shaderManager->addResources(std::move(shaders));


    // adding  meshes
    auto meshManager = std::make_unique<MeshManager>();
    std::vector<std::unique_ptr<Mesh>> meshes;
    meshes.push_back(std::make_unique<Mesh>(
        "cube",
        "../res/meshes/cube.obj"
    ));
    meshes.push_back(std::make_unique<Mesh>(
        "sphere",
        "../res/meshes/sphere.obj"
    ));
    // meshes.push_back(std::make_unique<Mesh>(
    //     "sphere",
    //     "../res/meshes/sphere2.obj"
    // ));
    meshManager->addResources(std::move(meshes));


    // adding textures
    auto textureManager = std::make_unique<TextureManager>();
    std::vector<std::unique_ptr<Texture>> textures;
    textures.push_back(std::make_unique<Texture>(
        "dirtblock",
        "../res/textures/dirtblock.jpg"
    ));
    textureManager->addResources(std::move(textures));


    // create camera
    float FOV = 45.0f;
    float nearPlane = 0.1f;
    float farPlane = 100.0f;
    float aspectRatio = static_cast<float>(m_screenWidth) / static_cast<float>(m_screenHeight);
    auto camera = std::make_unique<Camera>(
        glm::vec3(0.0f, 8.5f,  30.0f),
        glm::vec3(0.0f, 0.0f, -1.0f),
        glm::vec3(0.0f, 1.0f,  0.0f),
        FOV,
        aspectRatio,
        nearPlane,
        farPlane,
        m_window
    );

    // TODO : REFACTOR SUCH THAT IT CAN CREATE MULTIPLE SCENE ENVIRONMENTS THAT CAN BE SELECTED
    // create scene
    m_scene = std::make_unique<Scene>(
        "Test Scene",
        std::move(shaderManager),
        std::move(meshManager),
        std::move(textureManager),
        std::move(camera)
    );


    // create debug window
    const char* glslVersion = "#version 330";
    m_debugWindow = std::make_unique<DebugWindow>(
        m_window,
        glslVersion
    );

    std::cout << "ImGuiWindow created.\n";
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

        m_timer->capFrameRate(m_targetFPS);
    }
}

void PhysicsEngine::close()
{
    m_debugWindow->close();
    m_scene->clear();
    glfwTerminate();
    std::cout << "PhysicsEngine closed.\n";
}