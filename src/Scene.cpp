#include "Scene.hpp"

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/string_cast.hpp>

Scene::Scene(
    const std::string& name,
    std::unique_ptr<ShaderManager> shaderManager,
    std::unique_ptr<MeshManager> meshManager,
    std::unique_ptr<TextureManager> textureManager,
    std::unique_ptr<Camera> camera
)
    :   m_name(name),
        m_shaderManager(std::move(shaderManager)),
        m_meshManager(std::move(meshManager)),
        m_textureManager(std::move(textureManager)),
        m_camera(std::move(camera)),
        m_gravitationalAcceleration(0.0f, 0.0f, 0.0f)
{
    Shader platformShader = m_shaderManager->getShader("platform");
    Shader dirtBlockShader = m_shaderManager->getShader("dirtblock");
    Shader sphereShader = m_shaderManager->getShader("sphere");

    Mesh cubeMesh = m_meshManager->getMesh("cube");
    // Mesh sphereMesh = m_meshManager->getMesh("sphere");

    Texture dirtBlockTexture = m_textureManager->getTexture("dirtblock");

    // platform
    Transform platformTransform;
    platformTransform.setProjection(*m_camera);
    glm::vec3 platformPosition(0.0f, -0.5f, 0.0f);
    glm::mat4 platformTranslationMatrix = glm::translate(
        glm::mat4(1.0f),
        platformPosition
    );
    platformTranslationMatrix = glm::scale(
        platformTranslationMatrix,
        glm::vec3(5.0f, 0.5f, 2.5f)
    );
    platformTransform.setModel(platformTranslationMatrix);
    platformTransform.setView(*m_camera);
    auto platformBlock = std::make_unique<Cube>(
        "Platform",
        platformTransform,
        platformShader,
        cubeMesh
    );
    m_objects.push_back(std::move(platformBlock));


    // dirtBlock
    Transform dirtBlockTransform;
    dirtBlockTransform.setProjection(*m_camera);
    glm::vec3 dirtBlockPosition(0.0f, 0.5f, 0.0f);
    glm::mat4 dirtBlockTranslationMatrix = glm::translate(
        glm::mat4(1.0f),
        dirtBlockPosition
    );
    dirtBlockTranslationMatrix = glm::rotate(
        dirtBlockTranslationMatrix,
        glm::radians(0.0f),
        glm::vec3(0.0f, 1.0f, 1.0f)
    );
    dirtBlockTranslationMatrix = glm::scale(
        dirtBlockTranslationMatrix,
        glm::vec3(0.5f, 0.5f, 0.5f)
    );
    dirtBlockTransform.setModel(dirtBlockTranslationMatrix);
    dirtBlockTransform.setView(*m_camera);
    auto dirtBlock = std::make_unique<DirtBlock>(
        "DirtBlock",
        dirtBlockTransform,
        dirtBlockShader,
        cubeMesh,
        dirtBlockTexture,
        false
    );
    m_objects.push_back(std::move(dirtBlock));


    // // sphere
    // Transform sphereTransform;
    // sphereTransform.setProjection(*m_camera);
    // glm::vec3 spherePosition(2.0f, 3.0f, 0.0f);
    // glm::mat4 sphereTranslationMatrix = glm::translate(
    //     glm::mat4(1.0f),
    //     spherePosition
    // );
    // sphereTranslationMatrix = glm::scale(
    //     sphereTranslationMatrix,
    //     glm::vec3(0.3f, 0.3f, 0.3f)
    // );
    // sphereTransform.setModel(sphereTranslationMatrix);
    // sphereTransform.setView(*m_camera);
    // auto sphere = std::make_unique<Sphere>(
    //     "Sphere",
    //     sphereTransform,
    //     sphereShader,
    //     sphereMesh,
    //     false
    // );
    // m_objects.push_back(std::move(sphere));

    std::cout << name << " created.\n";
}

void Scene::applyGravity(
    Object& object,
    float deltaTime
)
{
    for (auto& vertexTransform : object.getVertexTransforms())
    {
        vertexTransform.setAcceleration(m_gravitationalAcceleration);
    }
}

void Scene::applyPBD(
    Object& object,
    float deltaTime
)
{
    std::vector<glm::vec3> xTilde;
    for (auto& vertexTransform : object.getVertexTransforms())
    {
        glm::vec3 predictedPosition = vertexTransform.getPosition() +
                                    deltaTime * vertexTransform.getVelocity() +
                                    deltaTime * deltaTime * vertexTransform.getAcceleration();
        xTilde.push_back(predictedPosition);
    }

    std::vector<glm::vec3> x0 = xTilde;
    std::vector<float> lambda0(xTilde.size(), 0.0f);

    const int solverIterations = 100;
    int i = 0;
    while (i < solverIterations)
    {
        i++;
    }

}

void Scene::update(float deltaTime)
{
    m_camera->setDeltaTime(deltaTime);
    m_camera->move();

    for (auto& object : m_objects)
    {
        Transform& transform = object->getTransform();
        transform.setView(*m_camera);

        if (!object->isStatic())
        {
            applyGravity(*object, deltaTime);
            applyPBD(*object, deltaTime);
        }

        object->update(deltaTime);

    }
}

void Scene::render()
{
    glEnable(GL_DEPTH_TEST);

    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glFrontFace(GL_CCW);

    glClearColor(0.5f, 0.1f, 0.4f, 1.0f); // background
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    for (const auto& object : m_objects)
    {
        object->render();
    }
}

void Scene::clear()
{
    m_textureManager->deleteAllTextures();
    m_meshManager->deleteAllMeshes();
    m_shaderManager->deleteAllShaders();
    m_objects.clear();

    std::cout << m_name << " cleared.\n";
}