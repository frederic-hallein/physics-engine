#include "Scene.hpp"

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/string_cast.hpp>

Shader Object::s_vertexNormalShader;
Shader Object::s_faceNormalShader;

void Scene::createObjects()
{
    Object::setVertexNormalShader(m_shaderManager->getResource("vertexNormal"));
    Object::setFaceNormalShader(m_shaderManager->getResource("faceNormal"));

    Shader platformShader = m_shaderManager->getResource("platform");
    Shader lightShader = m_shaderManager->getResource("light");
    Shader dirtBlockShader = m_shaderManager->getResource("dirtblock");
    // Shader sphereShader = m_shaderManager->getResource("sphere");

    Mesh cubeMesh = m_meshManager->getResource("cube");
    // Mesh sphereMesh = m_meshManager->getResource("sphere");

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
    glm::vec3 platformPosition(-0.0f, -0.5f, 0.0f);
    glm::mat4 platformTranslationMatrix = glm::translate(
        glm::mat4(1.0f),
        platformPosition
    );
    platformTranslationMatrix = glm::rotate(
        platformTranslationMatrix,
        glm::radians(-0.0f),
        glm::vec3(0.0f, 0.0f, 1.0f)
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
        // vertexNormalShader,
        cubeMesh
    );
    m_objects.push_back(std::move(platformBlock));

    // Transform platformTransform2;
    // platformTransform2.setProjection(*m_camera);
    // glm::vec3 platformPosition2(8.0f, 8.0f, 0.0f);
    // glm::mat4 platformTranslationMatrix2 = glm::translate(
    //     glm::mat4(1.0f),
    //     platformPosition2
    // );
    // platformTranslationMatrix2 = glm::rotate(
    //     platformTranslationMatrix2,
    //     glm::radians(30.0f),
    //     glm::vec3(0.0f, 0.0f, 1.0f)
    // );
    // platformTranslationMatrix2 = glm::scale(
    //     platformTranslationMatrix2,
    //     glm::vec3(10.0f, 0.5f, 10.0f)
    // );
    // platformTransform2.setModel(platformTranslationMatrix2);
    // platformTransform2.setView(*m_camera);
    // auto platformBlock2 = std::make_unique<Object>(
    //     "Platform",
    //     platformTransform2,
    //     m_k,
    //     platformShader,
    //     normalShader,
    //     cubeMesh
    // );
    // m_objects.push_back(std::move(platformBlock2));


    // dirtBlock
    Transform dirtBlockTransform;
    dirtBlockTransform.setProjection(*m_camera);
    glm::vec3 dirtBlockPosition(-0.0f, 5.0f, 0.0f);
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
        cubeMesh,
        dirtBlockTexture,
        false
    );
    m_objects.push_back(std::move(dirtBlock));

    // // sphere
    // Transform sphereTransform;
    // sphereTransform.setProjection(*m_camera);
    // glm::vec3 spherePosition(0.0f, 7.5f, 0.0f);
    // glm::mat4 sphereTranslationMatrix = glm::translate(
    //     glm::mat4(1.0f),
    //     spherePosition
    // );
    // sphereTranslationMatrix = glm::scale(
    //     sphereTranslationMatrix,
    //     glm::vec3(1.0f, 1.0f, 1.0f)
    // );
    // sphereTransform.setModel(sphereTranslationMatrix);
    // sphereTransform.setView(*m_camera);
    // auto sphere = std::make_unique<Object>(
    //     "Sphere",
    //     sphereTransform,
    //     m_k,
    //     sphereShader,
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
        mesh->setCandidateMeshes(m_meshPtrs);
        mesh->constructEnvCollisionConstraints();
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
    const Mesh::DistanceConstraints& distanceConstraints
)
{
    size_t edgesSize = distanceConstraints.edges.size();
    size_t CSize = distanceConstraints.C.size();
    size_t gradCSize = distanceConstraints.gradC.size();
    if (edgesSize != gradCSize)
    {
        std::cerr << "DistanceConstraints size mismatch:\n"
                  << "constraints = " << CSize << ", "
                  << "gradConstraints = " << gradCSize << ", "
                  << "edgeVertices = " << edgesSize << std::endl;
        return;
    }

    for (size_t j = 0; j < distanceConstraints.edges.size(); ++j)
    {
        float C_j = distanceConstraints.C[j](x);
        if (std::abs(C_j) > 1e-6f)
        {
            std::vector<glm::vec3> gradC_j = distanceConstraints.gradC[j](x);

            const Edge& edge = distanceConstraints.edges[j];
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
    const Mesh::VolumeConstraints& volumeConstraints
)
{
    size_t trianglesSize = volumeConstraints.triangles.size();
    size_t CSize = volumeConstraints.C.size();
    size_t gradCSize = volumeConstraints.gradC.size();
    if (trianglesSize != gradCSize)
    {
        std::cerr << "VolumeConstraints size mismatch:\n"
                  << "constraints = " << CSize << ", "
                  << "gradConstraints = " << gradCSize << ", "
                  << "triangleVertices = " << trianglesSize << std::endl;
        return;
    }

    for (size_t j = 0; j < volumeConstraints.triangles.size(); ++j)
    {
        float C_j = volumeConstraints.C[0](x);
        if (std::abs(C_j) > 1e-6f)
        {
            std::vector<glm::vec3> gradC_j = volumeConstraints.gradC[j](x);

            const Triangle& tri = volumeConstraints.triangles[j];
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

void Scene::solveEnvCollisionConstraints(
    std::vector<glm::vec3>& x,
    const std::vector<glm::vec3>& posDiff,
    const std::vector<float>& M,
    float alphaTilde,
    float gamma,
    std::vector<Mesh::EnvCollisionConstraints> perEnvCollisionConstraints
)
{
    // TODO
}

void Scene::applyPBD(
    Object& object,
    float deltaTime
)
{
    const auto& mesh = object.getMesh();
    const auto& distanceConstraints = mesh.distanceConstraints;
    const auto& volumeConstraints = mesh.volumeConstraints;
    const auto& perEnvCollisionConstraints = mesh.perEnvCollisionConstraints;

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
                distanceConstraints
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
                volumeConstraints
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
                perEnvCollisionConstraints
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
    m_textureManager->deleteAllResources();
    m_meshManager->deleteAllResources();
    m_shaderManager->deleteAllResources();
    m_objects.clear();

    std::cout << m_name << " cleared.\n";
}