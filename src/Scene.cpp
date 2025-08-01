#include "Scene.hpp"

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/string_cast.hpp>

void Scene::createObjects()
{
    // Shader normalShader = m_shaderManager->getShader("normal");
    // Shader platformShader = m_shaderManager->getShader("platform");
    // Shader lightShader = m_shaderManager->getShader("light");
    // Shader dirtBlockShader = m_shaderManager->getShader("dirtblock");
    // Shader sphereShader = m_shaderManager->getShader("sphere");

    Shader normalShader = m_shaderManager->getResource("normal");
    Shader platformShader = m_shaderManager->getResource("platform");
    Shader lightShader = m_shaderManager->getResource("light");
    Shader dirtBlockShader = m_shaderManager->getResource("dirtblock");
    Shader sphereShader = m_shaderManager->getResource("sphere");

    // Mesh cubeMesh = m_meshManager->getMesh("cube");
    Mesh cubeMesh = m_meshManager->getResource("cube");
    // Mesh sphereMesh = m_meshManager->getMesh("sphere");

    // Texture dirtBlockTexture = m_textureManager->getTexture("dirtblock");
    Texture dirtBlockTexture = m_textureManager->getResource("dirtblock");

    // // light
    // Transform lightTransform;
    // lightTransform.setProjection(*m_camera);
    // glm::vec3 lightPosition(0.0f, 4.0f, 0.0f);
    // glm::mat4 lightTranslationMatrix = glm::translate(
    //     glm::mat4(1.0f),
    //     lightPosition
    // );
    // lightTranslationMatrix = glm::scale(
    //     lightTranslationMatrix,
    //     glm::vec3(0.01f, 0.01f, 0.01f)
    // );
    // lightTransform.setModel(lightTranslationMatrix);
    // lightTransform.setView(*m_camera);
    // auto lightBlock = std::make_unique<Cube>(
    //     "Light",
    //     m_k,
    //     lightTransform,
    //     lightShader,
    //     sphereMesh
    // );
    // m_objects.push_back(std::move(lightBlock));

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
        glm::vec3(10.0f, 0.5f, 10.0f)
    );
    platformTransform.setModel(platformTranslationMatrix);
    platformTransform.setView(*m_camera);
    auto platformBlock = std::make_unique<Object>(
        "Platform",
        platformTransform,
        m_k,
        platformShader,
        normalShader,
        cubeMesh
    );
    m_objects.push_back(std::move(platformBlock));


    // dirtBlock
    Transform dirtBlockTransform;
    dirtBlockTransform.setProjection(*m_camera);
    glm::vec3 dirtBlockPosition(-0.0f, 4.0f, 0.0f);
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
        glm::vec3(1.0f, 1.0f, 1.0f)
    );
    dirtBlockTransform.setModel(dirtBlockTranslationMatrix);
    dirtBlockTransform.setView(*m_camera);
    auto dirtBlock = std::make_unique<Object>(
        "DirtBlock",
        dirtBlockTransform,
        m_k,
        dirtBlockShader,
        normalShader,
        cubeMesh,
        dirtBlockTexture,
        false
    );
    m_objects.push_back(std::move(dirtBlock));

    // // sphere
    // Transform sphereTransform;
    // sphereTransform.setProjection(*m_camera);
    // glm::vec3 spherePosition(5.0f, 7.5f, 0.0f);
    // glm::mat4 sphereTranslationMatrix = glm::translate(
    //     glm::mat4(1.0f),
    //     spherePosition
    // );
    // sphereTranslationMatrix = glm::scale(
    //     sphereTranslationMatrix,
    //     glm::vec3(3.0f, 3.0f, 3.0f)
    // );
    // sphereTransform.setModel(sphereTranslationMatrix);
    // sphereTransform.setView(*m_camera);
    // auto sphere = std::make_unique<Object>(
    //     "Sphere",
    //     sphereTransform,
    //     m_k,
    //     sphereShader,
    //     normalShader,
    //     sphereMesh,
    //     std::nullopt,
    //     false
    // );
    // m_objects.push_back(std::move(sphere));
}

void Scene::setupMeshEnvCollisionConstraints()
{
    m_meshPtrs.reserve(m_objects.size());
    for (const auto& obj : m_objects)
    {
        m_meshPtrs.push_back(&obj->getMesh());
    }

    for (auto* mesh : m_meshPtrs)
    {
        mesh->setCandidateVertices(m_meshPtrs);
        mesh->constructEnvCollisionConstraints();
        mesh->constructGradEnvCollisionConstraints();
    }

    for (auto* mesh : m_meshPtrs)
    {
        mesh->updateEnvCollisionConstraintVertices();
    }
}

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
        m_gravitationalAcceleration(0.0f),
        m_enableDistanceConstraints(true),
        m_enableVolumeConstraints(true),
        m_enableEnvCollisionConstraints(false),
        m_pbdSubsteps(5),
        m_alpha(0.001f),
        m_beta(5.0f),
        m_k(1.0f)
{
    createObjects();
    setupMeshEnvCollisionConstraints();

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

float Scene::calculateDeltaLambda(
    float C_j,
    const std::vector<glm::vec3>& gradC_j,
    const std::vector<glm::vec3>& posDiff,
    std::span<const unsigned int> constraintVertices,
    const std::vector<float>& M,
    float alphaTilde,
    float gamma
)
{
    float gradCMInverseGradCT = 0.0f;
    float gradCPosDiff = 0.0f;
    size_t n = constraintVertices.size();

    for (size_t i = 0; i < n; ++i)
    {
        unsigned int v = constraintVertices[i];
        float w = 1.0f / M[v];
        gradCMInverseGradCT += w * glm::dot(gradC_j[i], gradC_j[i]);
        gradCPosDiff += glm::dot(gradC_j[i], posDiff[v]);
    }

    return (-C_j - gamma * gradCPosDiff) / ((1 + gamma) * gradCMInverseGradCT + alphaTilde);
}

std::vector<glm::vec3> Scene::calculateDeltaX(
    float lambda,
    const std::vector<float>& M,
    std::vector<glm::vec3>& gradC_j,
    std::span<const unsigned int> constraintVertices
)
{
    std::vector<glm::vec3> deltaX(M.size(), glm::vec3(0.0f));
    size_t n = constraintVertices.size();
    for (size_t i = 0; i < n; ++i)
    {
        unsigned int v = constraintVertices[i];
        float w = 1.0f / M[v];
        deltaX[v] = lambda * w * gradC_j[i];
    }

    return deltaX;
}

void Scene::solveDistanceConstraints(
    std::vector<glm::vec3>& x,
    const std::vector<glm::vec3>& posDiff,
    const std::vector<float>& M,
    float alphaTilde,
    float gamma,
    const std::vector<std::function<float(const std::vector<glm::vec3>&)>>& distanceC,
    const std::vector<std::function<std::vector<glm::vec3>(const std::vector<glm::vec3>&)>>& gradDistanceC,
    const std::vector<Edge>& distanceConstraintVertices
)
{
    for (size_t j = 0; j < gradDistanceC.size(); ++j)
    {
        float C_j = distanceC[j](x);
        if (C_j < 0.0f || C_j > 0.0f)
        {
            std::vector<glm::vec3> gradC_j = gradDistanceC[j](x);

            const Edge& edge = distanceConstraintVertices[j];
            const std::array<unsigned int, 2> constraintVertices = { edge.v1, edge.v2 };

            float deltaLambda = calculateDeltaLambda(C_j, gradC_j, posDiff, constraintVertices, M, alphaTilde, gamma);
            std::vector<glm::vec3> deltaX = calculateDeltaX(deltaLambda, M, gradC_j, constraintVertices);

            for (size_t k = 0; k < deltaX.size(); ++k)
            {
                x[k] += deltaX[k];
            }
        }
    }
}

void Scene::solveVolumeConstraints(
    std::vector<glm::vec3>& x,
    const std::vector<glm::vec3>& posDiff,
    const std::vector<float>& M,
    float alphaTilde,
    float gamma,
    const std::vector<std::function<float(const std::vector<glm::vec3>&)>>& volumeC,
    const std::vector<std::function<std::vector<glm::vec3>(const std::vector<glm::vec3>&)>>& gradVolumeC,
    const std::vector<Triangle>& volumeConstraintVertices
)
{
    for (size_t j = 0; j < gradVolumeC.size(); ++j)
    {
        float C_j = volumeC[0](x);
        if (C_j < 0.0f || C_j > 0.0f)
        {
            std::vector<glm::vec3> gradC_j = gradVolumeC[j](x);

            const Triangle& tri = volumeConstraintVertices[j];
            const std::array<unsigned int, 3> constraintVertices = { tri.v1, tri.v2, tri.v3 };

            float deltaLambda = calculateDeltaLambda(C_j, gradC_j, posDiff, constraintVertices, M, alphaTilde, gamma);
            std::vector<glm::vec3> deltaX = calculateDeltaX(deltaLambda, M, gradC_j, constraintVertices);

            for (size_t k = 0; k < deltaX.size(); ++k)
            {
                x[k] += deltaX[k];
            }
        }
    }
}

// TODO : FIXME
void Scene::solveEnvCollisionConstraints(
    std::vector<glm::vec3>& x,
    const std::vector<glm::vec3>& posDiff,
    const std::vector<float>& M,
    float alphaTilde,
    float gamma,
    const std::vector<std::function<float(const std::vector<glm::vec3>&)>>& envCollisionC,
    const std::vector<std::function<std::vector<glm::vec3>(const std::vector<glm::vec3>&)>>& gradEnvCollisionC,
    const std::vector<unsigned int>& envCollisionConstraintVertices
)
{
    // std::cout << "solveEnvCollisionConstraints: M.size() = " << M.size()
    //           << ", envCollisionC.size() = " << envCollisionC.size()
    //           << ", gradEnvCollisionC.size() = " << gradEnvCollisionC.size()
    //           << ", envCollisionConstraintVertices.size() = " << envCollisionConstraintVertices.size()
    //           << std::endl;


    for (size_t j = 0; j < gradEnvCollisionC.size(); ++j)
    {
        float C_j = envCollisionC[j](x);
        if (C_j < 0.0f)
        {
            std::vector<glm::vec3> gradC_j = gradEnvCollisionC[j](x);

            const unsigned int& v = envCollisionConstraintVertices[j];
            const std::array<unsigned int, 1> constraintVertices = { v };

            float deltaLambda = calculateDeltaLambda(C_j, gradC_j, posDiff, constraintVertices, M, alphaTilde, gamma);
            std::vector<glm::vec3> deltaX = calculateDeltaX(deltaLambda, M, gradC_j, constraintVertices);

            for (size_t k = 0; k < deltaX.size(); ++k)
            {
                x[k] += deltaX[k];
            }
        }

    }
}

void Scene::applyPBD(
    Object& object,
    float deltaTime
)
{
    const auto& mesh = object.getMesh();
    const auto& distanceConstraintVertices = mesh.distanceConstraintVertices;
    const auto& volumeConstraintVertices = mesh.volumeConstraintVertices;
    const auto& envCollisionConstraintVertices = mesh.envCollisionConstraintVertices;
    const auto& distanceC = mesh.distanceConstraints;
    const auto& volumeC = mesh.volumeConstraints;
    const auto& envCollisionC = mesh.envCollisionConstraints;
    const auto& gradDistanceC = mesh.gradDistanceConstraints;
    const auto& gradVolumeC = mesh.gradVolumeConstraints;
    const auto& gradEnvCollisionC = mesh.gradEnvCollisionConstraints;
    auto& vertexTransforms = object.getVertexTransforms();

    const size_t numVerts = vertexTransforms.size();

    std::vector<float> M = object.getMass();
    std::vector<glm::vec3> x(numVerts, glm::vec3(0.0f));
    std::vector<glm::vec3> v(numVerts, glm::vec3(0.0f));
    std::vector<glm::vec3> a(numVerts, glm::vec3(0.0f));
    std::vector<glm::vec3> p(numVerts, glm::vec3(0.0f));
    std::vector<glm::vec3> posDiff(numVerts, glm::vec3(0.0f));
    std::vector<glm::vec3> deltaX(numVerts, glm::vec3(0.0f));

    int subStep = 1;
    const int n = m_pbdSubsteps;
    float deltaTime_s = deltaTime / static_cast<float>(n);

    float alphaTilde = m_alpha / (deltaTime_s * deltaTime_s);
    float betaTilde = (deltaTime_s * deltaTime_s) * m_beta;
    float gamma = (alphaTilde * betaTilde) / deltaTime_s;

    while (subStep < n + 1)
    {
        for (size_t i = 0; i < numVerts; ++i)
        {
            const Transform& vertexTransform = vertexTransforms[i];
            a[i] = vertexTransform.getAcceleration();
            v[i] = vertexTransform.getVelocity() + deltaTime_s * a[i];
            x[i] = vertexTransform.getPosition() + deltaTime_s * v[i];
            p[i] = vertexTransform.getPosition();
            posDiff[i] = x[i] - p[i];
        }

        // Distance constraints
        if (m_enableDistanceConstraints)
        {
            solveDistanceConstraints(
                x,
                posDiff,
                M,
                alphaTilde,
                gamma,
                distanceC,
                gradDistanceC,
                distanceConstraintVertices
            );
        }

        // Volume constraints
        if (m_enableVolumeConstraints)
        {
            solveVolumeConstraints(
                x,
                posDiff,
                M,
                alphaTilde,
                gamma,
                volumeC,
                gradVolumeC,
                volumeConstraintVertices
            );
        }

        // Environment Collision constraints
        if (m_enableEnvCollisionConstraints)
        {
            solveEnvCollisionConstraints(
                x,
                posDiff,
                M,
                alphaTilde,
                gamma,
                envCollisionC,
                gradEnvCollisionC,
                envCollisionConstraintVertices
            );
        }

        // Update positions and velocities
        for (size_t i = 0; i < numVerts; ++i)
        {
            Transform& vertexTransform = vertexTransforms[i];

            glm::vec3 newX = x[i];
            glm::vec3 newV = (newX - p[i]) / deltaTime_s;

            if (newX.y < 0.0f && newV.y < 0.0f)
            {
                newX.y = 0.0f;
                newV.y = 0.0f;
            }

            vertexTransform.setPosition(newX);
            vertexTransform.setVelocity(newV);
        }

        subStep++;
    }

}

void Scene::update(float deltaTime)
{
    m_camera->setDeltaTime(deltaTime);
    // m_camera->move();

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

    glClearColor(0.1f, 0.5f, 0.4f, 1.0f); // background
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    for (const auto& object : m_objects)
    {
        object->render();
    }

}

void Scene::clear()
{
    // m_textureManager->deleteAllTextures();
    // m_meshManager->deleteAllMeshes();
    // m_shaderManager->deleteAllShaders();
    m_textureManager->deleteAllResources();
    m_meshManager->deleteAllResources();
    m_shaderManager->deleteAllResources();
    m_objects.clear();

    std::cout << m_name << " cleared.\n";
}