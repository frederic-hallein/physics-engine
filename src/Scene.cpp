#include "Scene.hpp"

Shader Object::s_vertexNormalShader;
Shader Object::s_faceNormalShader;

void Scene::createObjects()
{
    Object::setVertexNormalShader(m_shaderManager->getResource("vertexNormal"));
    Object::setFaceNormalShader(m_shaderManager->getResource("faceNormal"));

    Shader platformShader = m_shaderManager->getResource("platform");
    Shader lightShader = m_shaderManager->getResource("light");
    Shader dirtBlockShader = m_shaderManager->getResource("dirtblock");
    Shader sphereShader = m_shaderManager->getResource("sphere");

    Mesh cubeMesh = m_meshManager->getResource("cube");
    Mesh sphereMesh = m_meshManager->getResource("sphere");

    Texture dirtBlockTexture = m_textureManager->getResource("dirtblock");

    // Platform definitions
    struct PlatformDef {
        std::string name;
        glm::vec3 position;
        glm::vec3 rotationAxis;
        float rotationDeg;
        glm::vec3 scale;
    };

    std::vector<PlatformDef> platforms = {
        { "Platform", glm::vec3(0.0f, -0.5f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f), 0.0f, glm::vec3(40.0f, 0.5f, 20.0f) },
        { "Wall",     glm::vec3(0.0f, 1.0f, -20.5f), glm::vec3(1.0f, 0.0f, 0.0f), 90.0f, glm::vec3(40.0f, 0.5f, 2.0f) },
        { "Wall",     glm::vec3(-40.5f, 1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f), 90.0f, glm::vec3(2.0f, 0.5f, 20.0f) },
        { "Ramp",     glm::vec3(10.0f, 10.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f), 30.0f, glm::vec3(15.0f, 0.5f, 6.0f) },
        { "Ramp",     glm::vec3(-10.0f, 20.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f), -30.0f, glm::vec3(15.0f, 0.5f, 6.0f) }
    };

    for (const auto& def : platforms)
    {
        Transform platformTransform;
        platformTransform.setProjection(*m_camera);

        glm::mat4 model = glm::translate(glm::mat4(1.0f), def.position);
        model = glm::rotate(model, glm::radians(def.rotationDeg), def.rotationAxis);
        model = glm::scale(model, def.scale);

        platformTransform.setModel(model);
        platformTransform.setView(*m_camera);

        auto platformBlock = std::make_unique<Object>(
            def.name,
            platformTransform,
            m_k,
            platformShader,
            cubeMesh
        );
        m_objects.push_back(std::move(platformBlock));
    }

    // // dirtBlock
    // Transform dirtBlockTransform;
    // dirtBlockTransform.setProjection(*m_camera);
    // glm::vec3 dirtBlockPosition(-0.0f, 25.0f, 0.0f);
    // glm::mat4 dirtBlockTranslationMatrix = glm::translate(
    //     glm::mat4(1.0f),
    //     dirtBlockPosition
    // );
    // dirtBlockTranslationMatrix = glm::rotate(
    //     dirtBlockTranslationMatrix,
    //     glm::radians(45.0f),
    //     glm::vec3(0.0f, 1.0f, 1.0f)
    // );
    // dirtBlockTranslationMatrix = glm::scale(
    //     dirtBlockTranslationMatrix,
    //     glm::vec3(1.0f, 1.0f, 1.0f)
    // );
    // dirtBlockTransform.setModel(dirtBlockTranslationMatrix);
    // dirtBlockTransform.setView(*m_camera);
    // auto dirtBlock = std::make_unique<Object>(
    //     "Block",
    //     dirtBlockTransform,
    //     m_k,
    //     dirtBlockShader,
    //     cubeMesh,
    //     std::nullopt,
    //     false
    // );
    // m_objects.push_back(std::move(dirtBlock));

    // // Dirt block definitions
    // struct DirtBlockDef {
    //     std::string name;
    //     glm::vec3 position;
    //     glm::vec3 rotationAxis;
    //     float rotationDeg;
    //     glm::vec3 scale;
    // };

    // std::vector<DirtBlockDef> dirtBlocks = {
    //     { "Block1", glm::vec3(-0.0f, 25.0f, 0.0f), glm::vec3(0.0f, 1.0f, 1.0f), 45.0f, glm::vec3(1.0f, 1.0f, 1.0f) },
    //     { "Block2", glm::vec3(5.0f, 27.0f, 2.0f), glm::vec3(1.0f, 0.0f, 0.0f), 30.0f, glm::vec3(1.0f, 1.0f, 1.0f) },
    //     { "Block3", glm::vec3(-3.0f, 23.0f, -4.0f), glm::vec3(0.0f, 0.0f, 1.0f), 60.0f, glm::vec3(1.0f, 1.0f, 1.0f) },
    //     { "Block4", glm::vec3(8.0f, 26.0f, -3.0f), glm::vec3(0.0f, 1.0f, 0.0f), 15.0f, glm::vec3(1.0f, 1.0f, 1.0f) },
    //     { "Block5", glm::vec3(-6.0f, 24.0f, 5.0f), glm::vec3(1.0f, 1.0f, 0.0f), 75.0f, glm::vec3(1.0f, 1.0f, 1.0f) }
    // };

    // for (const auto& def : dirtBlocks)
    // {
    //     Transform dirtBlockTransform;
    //     dirtBlockTransform.setProjection(*m_camera);

    //     glm::mat4 model = glm::translate(glm::mat4(1.0f), def.position);
    //     model = glm::rotate(model, glm::radians(def.rotationDeg), def.rotationAxis);
    //     model = glm::scale(model, def.scale);

    //     dirtBlockTransform.setModel(model);
    //     dirtBlockTransform.setView(*m_camera);

    //     auto dirtBlock = std::make_unique<Object>(
    //         def.name,
    //         dirtBlockTransform,
    //         m_k,
    //         dirtBlockShader,
    //         cubeMesh,
    //         dirtBlockTexture,
    //         false
    //     );
    //     m_objects.push_back(std::move(dirtBlock));
    // }

    // sphere
    Transform sphereTransform;
    sphereTransform.setProjection(*m_camera);
    glm::vec3 spherePosition(-0.0f, 25.0f, 0.0f);
    glm::mat4 sphereTranslationMatrix = glm::translate(
        glm::mat4(1.0f),
        spherePosition
    );
    sphereTranslationMatrix = glm::scale(
        sphereTranslationMatrix,
        glm::vec3(3.0f, 3.0f, 3.0f)
    );
    sphereTransform.setModel(sphereTranslationMatrix);
    sphereTransform.setView(*m_camera);
    auto sphere = std::make_unique<Object>(
        "Sphere",
        sphereTransform,
        m_k,
        sphereShader,
        sphereMesh,
        std::nullopt,
        false
    );
    m_objects.push_back(std::move(sphere));
}

void Scene::setupEnvCollisionConstraints()
{
    for (const auto& obj : m_objects)
    {
        if (obj->isStatic()) continue;

        std::vector<Object*> candidateObjects;
        candidateObjects.reserve(m_objects.size());
        for (const auto& objPtr : m_objects) {
            candidateObjects.push_back(objPtr.get());
        }

        Mesh& mesh = obj->getMesh();
        mesh.setCandidateObjectMeshes(candidateObjects);
        mesh.constructEnvCollisionConstraints();
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
        m_enableEnvCollisionConstraints(true),
        m_pbdSubsteps(10),
        m_alpha(0.001f),
        m_beta(5.0f),
        m_k(1.0f)
{
    createObjects();
    setupEnvCollisionConstraints();

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
        std::vector<glm::vec3> gradC_j = distanceConstraints.gradC[j](x);
        const Edge& edge = distanceConstraints.edges[j];
        const std::array<unsigned int, 2> constraintVertices = { edge.v1, edge.v2 };

        float deltaLambda = calculateDeltaLambda(C_j, gradC_j, posDiff, constraintVertices, M, alphaTilde, gamma);
        std::vector<glm::vec3> deltaX = calculateDeltaX(deltaLambda, M, gradC_j, constraintVertices);

        {
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

void Scene::solveEnvCollisionConstraints(
    std::vector<glm::vec3>& x,
    const std::vector<glm::vec3>& posDiff,
    const std::vector<float>& M,
    float alphaTilde,
    float gamma,
    std::vector<Mesh::EnvCollisionConstraints> perEnvCollisionConstraints
)
{
    for (size_t setIdx = 0; setIdx < perEnvCollisionConstraints.size(); ++setIdx)
    {
        const auto& constraints = perEnvCollisionConstraints[setIdx];
        size_t verticesSize = constraints.vertices.size();
        size_t CSize = constraints.C.size();
        size_t gradCSize = constraints.gradC.size();

        if (verticesSize != gradCSize)
        {
            std::cerr << "EnvCollisionConstraints size mismatch in set " << setIdx << std::endl;
            continue;
        }

        for (const auto& [vertex, constraintIndices] : constraints.vertexToConstraints)
        {
            bool allNegative = true;
            float maxNegativeC = -std::numeric_limits<float>::max(); // Initialize to most negative possible value
            size_t maxIdx = 0;
            for (size_t idx : constraintIndices)
            {
                float C_j = constraints.C[idx](x);
                if (C_j >= 0.0f)
                {
                    allNegative = false;
                }

                // Track the constraint with biggest negative value
                if (C_j < 0.0f && C_j > maxNegativeC)
                {
                    maxNegativeC = C_j;
                    maxIdx = idx;
                }
            }

            // If all constraints are negative, we have a collision with this vertex
            if (allNegative && !constraintIndices.empty())
            {
                float C_j = maxNegativeC;
                std::vector<glm::vec3> gradC_j = constraints.gradC[maxIdx](x);

                std::array<unsigned int, 1> constraintVertices = { vertex };

                float deltaLambda = calculateDeltaLambda(C_j, gradC_j, posDiff, constraintVertices, M, alphaTilde, gamma);
                std::vector<glm::vec3> deltaX = calculateDeltaX(deltaLambda, M, gradC_j, constraintVertices);

                for (size_t k = 0; k < deltaX.size(); ++k)
                {
                    x[k] += deltaX[k];
                }
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

    float alphaTilde;
    float betaTilde;
    float gamma;

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

        // Environment Collision constraints
        if (m_enableEnvCollisionConstraints)
        {
            alphaTilde = 0.0f;
            betaTilde = 0.0f;
            gamma = 0.0f;
            solveEnvCollisionConstraints(
                x,
                posDiff,
                M,
                alphaTilde,
                gamma,
                perEnvCollisionConstraints
            );
        }

        // Distance constraints
        if (m_enableDistanceConstraints)
        {
            alphaTilde = m_alpha / (deltaTime_s * deltaTime_s);
            betaTilde = (deltaTime_s * deltaTime_s) * m_beta;
            gamma = (alphaTilde * betaTilde) / deltaTime_s;
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
            alphaTilde = m_alpha / (deltaTime_s * deltaTime_s);
            betaTilde = (deltaTime_s * deltaTime_s) * m_beta;
            gamma = (alphaTilde * betaTilde) / deltaTime_s;
            solveVolumeConstraints(
                x,
                posDiff,
                M,
                alphaTilde,
                gamma,
                volumeConstraints
            );
        }

        // Update positions and velocities
        for (size_t i = 0; i < numVerts; ++i)
        {
            Transform& vertexTransform = vertexTransforms[i];

            glm::vec3 newX = x[i];
            glm::vec3 newV = (newX - p[i]) / deltaTime_s;

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

    // gravity and PBD
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