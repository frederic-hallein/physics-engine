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
        m_camera(std::move(camera))
{
    Shader& platformShader = m_shaderManager->getShader("platform");
    Shader& dirtBlockShader = m_shaderManager->getShader("dirtblock");

    Mesh& cubeMesh = m_meshManager->getMesh("cube");

    Texture& dirtBlockTexture = m_textureManager->getTexture("dirtblock");

    // platform
    Transform platformTransform;
    platformTransform.setProjection(
        m_camera->getFOV(),
        m_camera->getAspectRatio(),
        m_camera->getNearPlane(),
        m_camera->getFarPlane()
    );
    float separationDistance = 1.0f;
    for (int z = -15; z < 15; ++z)
    {
        for (int x = -15; x < 15; ++x)
        {
            // Calculate the new position for the platform block
            glm::vec3 newPosition(x * separationDistance, 0.0f, z * separationDistance);
            glm::mat4 translationMatrix = glm::translate(glm::mat4(1.0f), newPosition);
            platformTransform.setModel(translationMatrix);

            auto platformBlock = std::make_unique<Cube>(
                platformTransform,
                platformShader,
                cubeMesh
            );

            m_objects.push_back(std::move(platformBlock));
        }
    }

    // dirtBlock
    Transform dirtBlockTransform;
    dirtBlockTransform.setProjection(
        m_camera->getFOV(),
        m_camera->getAspectRatio(),
        m_camera->getNearPlane(),
        m_camera->getFarPlane()
    );
    glm::vec3 dirtBlockPosition(0.0f, 10.0f, 0.0f);
    glm::mat4 dirtBlockTranslationMatrix = glm::translate(
        glm::mat4(1.0f),
        dirtBlockPosition
    );
    dirtBlockTransform.setModel(dirtBlockTranslationMatrix);
    auto dirtBlock = std::make_unique<DirtBlock>(
        dirtBlockTransform,
        dirtBlockShader,
        cubeMesh,
        dirtBlockTexture,
        false
    );
    m_objects.push_back(std::move(dirtBlock));

}

void Scene::applyGravity(Transform& transform, float deltaTime)
{
    const glm::vec3 gravity(0.0f, -0.5f, 0.0f);

    // Update velocity with gravity
    glm::vec3 velocity = transform.getVelocity();
    velocity += gravity * deltaTime;
    transform.setVelocity(velocity);

    // Update position based on velocity
    glm::vec3 position = transform.getPosition();
    position += velocity * deltaTime;
    transform.setPosition(position);

    // Update the model matrix with the new position
    glm::mat4 translationMatrix = glm::translate(glm::mat4(1.0f), position);
    transform.setModel(translationMatrix);
}

void Scene::update(float deltaTime)
{
    m_camera->setDeltaTime(deltaTime);
    m_camera->move();

    // TODO : print out in ImGui
    // std::cout << "Camera Position: " << glm::to_string(m_camera->getPosition()) << '\n';

    for (auto& object : m_objects)
    {
        for (auto& vertexTransform : object->getVertexTransforms())
        {
            vertexTransform.setView(
                m_camera->getPosition(),
                m_camera->getFront(),
                m_camera->getUp()
            );

            if (!vertexTransform.isStatic())
                applyGravity(vertexTransform, deltaTime);

        }
    }
}

void Scene::render(float deltaTime)
{
    glEnable(GL_DEPTH_TEST);

    // // TODO : add key shortcut
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glFrontFace(GL_CW);

    glClearColor(0.2f, 0.2f, 0.8f, 1.0f); // background
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    for (auto& object : m_objects)
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
}