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
    Shader lightShader = m_shaderManager->getShader("light");
    Shader dirtBlockShader = m_shaderManager->getShader("dirtblock");
    Shader sphereShader = m_shaderManager->getShader("sphere");

    Mesh cubeMesh = m_meshManager->getMesh("cube");
    Mesh sphereMesh = m_meshManager->getMesh("sphere");

    Texture dirtBlockTexture = m_textureManager->getTexture("dirtblock");

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
        glm::vec3(50.0f, 0.1f, 50.0f)
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
    glm::vec3 dirtBlockPosition(-2.0f, 3.5f, 0.0f);
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

    // sphere
    Transform sphereTransform;
    sphereTransform.setProjection(*m_camera);
    glm::vec3 spherePosition(2.0f, 3.5f, 0.0f);
    glm::mat4 sphereTranslationMatrix = glm::translate(
        glm::mat4(1.0f),
        spherePosition
    );
    sphereTranslationMatrix = glm::scale(
        sphereTranslationMatrix,
        glm::vec3(1.0f, 1.0f, 1.0f)
    );
    sphereTransform.setModel(sphereTranslationMatrix);
    sphereTransform.setView(*m_camera);
    auto sphere = std::make_unique<Sphere>(
        "Sphere",
        sphereTransform,
        sphereShader,
        sphereMesh,
        false
    );
    m_objects.push_back(std::move(sphere));

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
    const std::vector<int>& constraintVertices,
    const std::vector<float>& M,
    float alphaTilde,
    float gamma
)
{
    float gradCMInverseGradCT = 0.0f;
    float gradCPosDiff = 0.0f;
    for (size_t i = 0; i < constraintVertices.size(); ++i)
    {
        int v = constraintVertices[i];
        gradCMInverseGradCT += (1.0f / M[v]) * glm::dot(gradC_j[i], gradC_j[i]);
        gradCPosDiff += glm::dot(gradC_j[i], posDiff[i]);
    }

    return (- C_j - gamma * gradCPosDiff) / ((1 + gamma) * gradCMInverseGradCT + alphaTilde);
}

std::vector<glm::vec3> Scene::calculateDeltaX(
    float lambda,
    const std::vector<float>& M,
    std::vector<glm::vec3>& gradC_j,
    const std::vector<int>& constraintVertices
)
{
    std::vector<glm::vec3> deltaX(M.size(), glm::vec3(0.0f));
    for (size_t i = 0; i < constraintVertices.size(); ++i)
    {
        int v = constraintVertices[i];
        deltaX[v] = lambda * (1.0f / M[v]) * gradC_j[i];
    }

    return deltaX;
}


void Scene::applyPBD(
    Object& object,
    float deltaTime
)
{


    std::vector<float> M = object.getMass();
    std::vector<std::vector<int>> distanceConstraintVertexPairs = object.getMesh().distanceConstraintVertices;
    std::vector<std::vector<int>> volumeConstraintVertexTriples = object.getMesh().volumeConstraintVertices;

    std::vector<std::function<float(const std::vector<glm::vec3>&)>> distanceC = object.getMesh().distanceConstraints;
    std::vector<std::function<float(const std::vector<glm::vec3>&)>> volumeC = object.getMesh().volumeConstraints;

    std::vector<std::function<std::vector<glm::vec3>(const std::vector<glm::vec3>&)>> gradDistanceC = object.getMesh().gradDistanceConstraints;
    std::vector<std::function<std::vector<glm::vec3>(const std::vector<glm::vec3>&)>> gradVolumeC = object.getMesh().gradVolumeConstraints;



    std::vector<glm::vec3> x(object.getVertexTransforms().size(), glm::vec3(0.0f));
    std::vector<glm::vec3> v(object.getVertexTransforms().size(), glm::vec3(0.0f));
    std::vector<glm::vec3> a(object.getVertexTransforms().size(), glm::vec3(0.0f));
    std::vector<glm::vec3> p(object.getVertexTransforms().size(), glm::vec3(0.0f));
    std::vector<glm::vec3> posDiff(object.getVertexTransforms().size(), glm::vec3(0.0f));

    int subStep = 1;
    const int n = 10;
    float deltaTime_s = deltaTime / (float)n;

    float alpha = 0.0001f;
    float beta = 0.0001f;

    float alphaTilde = alpha / (deltaTime_s * deltaTime_s);
    float betaTilde = (deltaTime_s * deltaTime_s) * beta;
    float gamma = (alphaTilde * betaTilde) / deltaTime_s;

    while (subStep < n + 1)
    {
        for (size_t i = 0; i < object.getVertexTransforms().size(); ++i)
        {
            Transform vertexTransform = object.getVertexTransforms()[i];

            a[i] = vertexTransform.getAcceleration();
            v[i] = vertexTransform.getVelocity() + deltaTime_s * a[i];
            x[i] = vertexTransform.getPosition() + deltaTime_s * v[i];
            p[i] = vertexTransform.getPosition();

            posDiff[i] = x[i] - p[i];
        }

        // // initialize differences of old and new positions and lambdas
        // std::vector<float> lambdaDifference(C.size(), 0.0f);
        // std::vector<float> positionDifference(x.size(), 0.0f);

        std::vector<glm::vec3> deltaX;

        // distance constraints
        for (size_t j = 0; j < distanceC.size(); ++j)
        {
            float C_j = distanceC[j](x);
            std::vector<glm::vec3> gradC_j = gradDistanceC[j](x);
            std::vector<int> distanceConstraintVertices = distanceConstraintVertexPairs[j];

            float deltaLambda = calculateDeltaLambda(C_j, gradC_j, posDiff, distanceConstraintVertices, M, alphaTilde, gamma);
            deltaX = calculateDeltaX(deltaLambda, M, gradC_j, distanceConstraintVertices);

            for (size_t i = 0; i < deltaX.size(); ++i)
            {
                x[i] += deltaX[i];
            }
        }

        // // volume constraint
        // for (size_t j = 0; j < volumeC.size(); ++j)
        // {
        //     float C_j = volumeC[j](x);
        //     std::vector<glm::vec3> gradC_j = gradVolumeC[j](x);
        //     std::vector<int> volumeConstraintVertices = volumeConstraintVertexTriples[j];

        //     float deltaLambda = calculateDeltaLambda(C_j, gradC_j, posDiff, volumeConstraintVertices, M, alphaTilde, gamma);
        //     deltaX = calculateDeltaX(deltaLambda, M, gradC_j, volumeConstraintVertices);

        //     for (size_t i = 0; i < deltaX.size(); ++i)
        //     {
        //         x[i] += deltaX[i];
        //     }
        // }


        // update position and velocities
        for (size_t i = 0; i < object.getVertexTransforms().size(); ++i)
        {
            Transform& vertexTransform = object.getVertexTransforms()[i];

            glm::vec3 newX = x[i];
            glm::vec3 newV = (newX - p[i]) / deltaTime_s;
            // glm::vec3 newA = a[i];

            if (newX.y < 0.0f && newV.y < 0.0f)
            {
                newX.y = 0.0f;
                newV.y = 0.0f;
            }

            vertexTransform.setPosition(newX);
            vertexTransform.setVelocity(newV);
            // vertexTransform.setAcceleration(newA);

        }

        subStep++;


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

    glClearColor(0.1f, 0.5f, 0.4f, 1.0f); // background
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