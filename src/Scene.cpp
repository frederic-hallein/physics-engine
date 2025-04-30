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
    // Create a transform for the platform block
    Shader& platformShader = m_shaderManager->getShader("platform");
    Mesh& cubeMesh = m_meshManager->getMesh("cube");
    Transform platformTransform;
    platformTransform.setProjection(
        m_camera->getFOV(),
        m_camera->getAspectRatio(),
        m_camera->getNearPlane(),
        m_camera->getFarPlane()
    );



    // Create a transform for the dirt block
    Shader& dirtBlockShader = m_shaderManager->getShader("dirtblock");
    Texture& dirtBlockTexture = m_textureManager->getTexture("dirtblock");
    Transform dirtBlockTransform;
    dirtBlockTransform.makeNoneStatic();
    dirtBlockTransform.setProjection(
        m_camera->getFOV(),
        m_camera->getAspectRatio(),
        m_camera->getNearPlane(),
        m_camera->getFarPlane()
    );
    glm::vec3 dirtBlockPosition(0.0f, 5.0f, 0.0f);
    glm::mat4 dirtBlockTranslationMatrix = glm::translate(glm::mat4(1.0f), dirtBlockPosition);
    dirtBlockTransform.setModel(dirtBlockTranslationMatrix);
    platformTransform.setView(
        m_camera->getPosition(),
        m_camera->getFront(),
        m_camera->getUp()
    );


    // create platformblock
    auto platformBlock = std::make_unique<Cube>(
        platformTransform,
        platformShader,
        cubeMesh
    );

    std::cout << "Platform Position: " << glm::to_string(platformTransform.getPosition()) << '\n';
    std::cout << "Camera Position: " << glm::to_string(m_camera->getPosition()) << '\n';


    // create dirtblock
    auto dirtBlock = std::make_unique<DirtBlock>(
        dirtBlockTransform,
        dirtBlockShader,
        cubeMesh,
        dirtBlockTexture
    );

    // Add objects to the scene
    m_objects.push_back(std::move(platformBlock));
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
        transform.setView(
            m_camera->getPosition(),
            m_camera->getFront(),
            m_camera->getUp()
        );

        if (!transform.isStatic())
        {
            // // Apply gravity (world translation)
            // glm::vec3 velocity = transform.getVelocity();
            // velocity += gravity * deltaTime;
            // transform.setVelocity(velocity);

            // glm::vec3 position = transform.getPosition();
            // position += velocity * deltaTime;
            // transform.setPosition(position);

            // glm::mat4 trans = glm::translate(
            //     glm::mat4(1.0f),
            //     velocity
            // );

            // glm::mat4 rot = glm::rotate(
            //     glm::mat4(1.0f),
            //     (float)glfwGetTime() * glm::radians(10.0f),
            //     glm::vec3(0.0f, 0.0f, 1.0f)
            // );

            // transform.setModel(trans * rot);

        }
        else
        {
            // std::cout << "Position: " << glm::to_string(transform.getPosition()) << '\n';
        }


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