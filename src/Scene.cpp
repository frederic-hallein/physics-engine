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
    glm::vec3 platformPosition(0.0f, -0.5f, 0.0f);
    glm::mat4 platformtranslationMatrix = glm::translate(glm::mat4(1.0f), platformPosition);
    platformtranslationMatrix = glm::scale(
        platformtranslationMatrix,
        glm::vec3(10.0f, 1.0f, 5.0f)
    );
    platformTransform.setModel(platformtranslationMatrix);
    auto platformBlock = std::make_unique<Cube>(
        platformTransform,
        platformShader,
        cubeMesh
    );
    m_objects.push_back(std::move(platformBlock));

    // dirtBlock
    Transform dirtBlockTransform;
    dirtBlockTransform.setProjection(
        m_camera->getFOV(),
        m_camera->getAspectRatio(),
        m_camera->getNearPlane(),
        m_camera->getFarPlane()
    );
    glm::vec3 dirtBlockPosition(0.0f, 1.0f, 0.0f);
    glm::mat4 dirtBlockTranslationMatrix = glm::translate(
        glm::mat4(1.0f),
        dirtBlockPosition
    );
    dirtBlockTranslationMatrix = glm::rotate(
        dirtBlockTranslationMatrix,
        45.0f,
        glm::vec3(0.7f, 0.5f, 1.0f)
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

void Scene::applyGravity(Object& object, float deltaTime)
{
    const glm::vec3 gravity(0.0f, -0.5f, 0.0f);

    // Iterate through the vertexTransforms of the object
    for (auto& vertexTransform : object.getVertexTransforms())
    {
        // Update velocity with gravity
        glm::vec3 velocity = vertexTransform.getVelocity();
        velocity += gravity * deltaTime;
        vertexTransform.setVelocity(velocity);

        // Update position based on velocity
        glm::vec3 position = vertexTransform.getPosition();

        if (position.y < 0.0f)
        {
            velocity = glm::vec3(0.0f);
        }

        position += velocity * deltaTime;

        // Extract the current rotation from the model matrix
        glm::mat4 currentModelMatrix = vertexTransform.getModelMatrix();
        glm::mat3 rotationMatrix = glm::mat3(currentModelMatrix);

        // Create a new translation matrix for the updated position
        glm::mat4 translationMatrix = glm::translate(glm::mat4(1.0f), position);

        // Combine the rotation and translation matrices
        glm::mat4 updatedModelMatrix = translationMatrix * glm::mat4(rotationMatrix);

        // Update the model matrix with the new position and preserved rotation
        vertexTransform.setModel(updatedModelMatrix);
        std::cout << "x_v = " << glm::to_string(vertexTransform.getPosition()) << '\n';
    }
}

// TODO : move this function to Transform class
void Scene::applyPBD(
    std::vector<Transform>& vertexTransforms,
    float deltaTime
)
{
    std::vector<glm::vec3> x;
    std::vector<glm::vec3> v;
    std::vector<float> w;
    for (const auto& vertexTransform : vertexTransforms)
    {
        x.push_back(vertexTransform.getPosition());
        v.push_back(vertexTransform.getVelocity());
        w.push_back(1.0f / vertexTransform.getMass());
    }

    glm::vec3 gravity(0.0f, -0.5f, 0.0f);
    for (size_t i = 0; i < vertexTransforms.size(); ++i)
    {
        v[i] += deltaTime * w[i] * gravity;
    }

    // dampVelocities(v1, ..., vN)

    std::vector<glm::vec3> p;
    for (size_t i = 0; i < vertexTransforms.size(); ++i)
    {
        p[i] += x[i] + deltaTime * v[i];
    }

    for (size_t i = 0; i < vertexTransforms.size(); ++i)
    {
        // generateCollisionConstraints(xi -> pi)
    }

    //while loop solverIterations
    //  projectConstraints(C1, ..., CM, p1, ..., pN)

    for (size_t i = 0; i < vertexTransforms.size(); ++i)
    {
        v[i] = (p[i] - x[i]) / deltaTime;
        x[i] = p[i];
    }

    //velocityUpdate(v1,..., VN);
}

void Scene::update(float deltaTime)
{
    m_camera->setDeltaTime(deltaTime);
    m_camera->move();

    // TODO : print out in ImGui
    // std::cout << "Camera Position: " << glm::to_string(m_camera->getPosition()) << '\n';

    for (auto& object : m_objects)
    {
        // look at center of objects
        Transform& transform = object->getTransform();
        transform.setView(
            m_camera->getPosition(),
            m_camera->getFront(),
            m_camera->getUp()
        );

        if (!object->isStatic())
        {
            applyGravity(*object, deltaTime);
            std::cout << "x_com = " << glm::to_string(transform.getPosition()) << '\n';
        }

        object->update();
    }
}

void Scene::render()
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