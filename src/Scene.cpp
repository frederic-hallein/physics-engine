#include "Scene.hpp"

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
    // create transform
    Transform transform;
    transform.setProjection(
        m_camera->getFOV(),
        m_camera->getAspectRatio(),
        m_camera->getNearPlane(),
        m_camera->getFarPlane()
    );

    glm::mat4 model = glm::rotate(
        transform.getModelMatrix(),
        glm::radians(180.0f),
        glm::vec3(2.0f, 1.0f, 0.5f)
    );
    transform.setModel(model);

    transform.setView(
        m_camera->getPosition(),
        m_camera->getFront(),
        m_camera->getUp()
    );

    // create dirtblock
    Shader& shader = m_shaderManager->getShader("dirtBlockShader");
    Mesh& mesh = m_meshManager->getMesh("cubeMesh");
    Texture& texture = m_textureManager->getTexture("dirtBlockTexture");

    auto dirtBlock = std::make_unique<DirtBlock>(
        transform,
        shader,
        mesh,
        texture
    );

    m_objects.push_back(std::move(dirtBlock));
}

void Scene::render(float deltaTime)
{
    glClearColor(0.2f, 0.2f, 0.8f, 1.0f); // background
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    m_camera->setDeltaTime(deltaTime);
    for (auto& object : m_objects)
    {
        Transform& transform = object->getTransform(); // Assuming Object has a getTransform() method
        transform.setView(
            m_camera->getPosition(),
            m_camera->getFront(),
            m_camera->getUp()
        );
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