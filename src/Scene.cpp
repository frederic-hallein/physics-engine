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

    glm::mat4 trans = glm::translate(
        glm::mat4(1.0f),
        glm::vec3(0.0f, 10.0f, 0.0f)
    );

    glm::mat4 rot = glm::rotate(
        glm::mat4(1.0f),
        glm::radians(0.0f),
        glm::vec3(0.0f, 0.0f, 1.0f)
    );

    transform.setModel(rot * trans);

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

void Scene::update(float deltaTime)
{
    const glm::vec3 gravity(0.0f, -1.0f, 0.0f);
    glm::vec3 velocity = glm::vec3(0.0f);

    m_camera->setDeltaTime(deltaTime);
    m_camera->move();

    for (auto& object : m_objects)
    {
        Transform& transform = object->getTransform();

        // Apply gravity (world translation)
        glm::vec3 velocity = transform.getVelocity();
        velocity += gravity * deltaTime;
        transform.setVelocity(velocity);

        glm::vec3 position = transform.getPosition();
        position += velocity * deltaTime;
        transform.setPosition(position);

        glm::mat4 trans = glm::translate(
            glm::mat4(1.0f),
            velocity
        );

        glm::mat4 rot = glm::rotate(
            glm::mat4(1.0f),
            (float)glfwGetTime() * glm::radians(10.0f),
            glm::vec3(0.0f, 0.0f, 1.0f)
        );

        transform.setModel(trans * rot);

        transform.setView(
            m_camera->getPosition(),
            m_camera->getFront(),
            m_camera->getUp()
        );


    }
}

void Scene::render(float deltaTime)
{
    // // TODO : add key shortcut
    // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

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