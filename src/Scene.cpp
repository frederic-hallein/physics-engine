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
    Mesh sphereMesh = m_meshManager->getMesh("sphere");

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
    glm::vec3 dirtBlockPosition(-2.0f, 3.0f, 0.0f);
    glm::mat4 dirtBlockTranslationMatrix = glm::translate(
        glm::mat4(1.0f),
        dirtBlockPosition
    );
    dirtBlockTranslationMatrix = glm::rotate(
        dirtBlockTranslationMatrix,
        glm::radians(30.0f),
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
    glm::vec3 spherePosition(2.0f, 3.0f, 0.0f);
    glm::mat4 sphereTranslationMatrix = glm::translate(
        glm::mat4(1.0f),
        spherePosition
    );
    sphereTranslationMatrix = glm::scale(
        sphereTranslationMatrix,
        glm::vec3(0.3f, 0.3f, 0.3f)
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

float Scene::calculateGradCMgradCT(const std::vector<glm::vec3>& gradC_j, const std::vector<std::vector<float>>& M)
{
    // rTerm = MInverse * gradCT
    size_t n = M.size();
    std::vector<glm::vec3> rTerm(gradC_j.size(), glm::vec3(0.0f));
    for (size_t n1 = 0; n1 < n; ++n1)
    {
        for (size_t n2 = 0; n2 < n; ++n2)
        {
            rTerm[n1] = M[n1][n2] * gradC_j[n2];
        }
    }

    // gradCMgradCT = gradC * rTerm
    float gradCMgradCT = 0.0f;
    for (size_t k = 0; k < gradC_j.size(); ++k)
    {
        gradCMgradCT += glm::dot(gradC_j[k], rTerm[k]);
    }

    return gradCMgradCT;
}

std::vector<glm::vec3> Scene::calculateDeltaX(
    const std::vector<std::vector<glm::vec3>>& gradCEval,
    std::vector<float> deltaLambda,
    const std::vector<std::vector<float>>& M
)
{
    // gradCT = gradCEval^T
    size_t rows = gradCEval.size();
    size_t cols = gradCEval[0].size();
    std::vector<std::vector<glm::vec3>> gradCT(cols, std::vector<glm::vec3>(rows));
    for (size_t n1 = 0; n1 < rows; ++n1) {
        for (size_t n2 = 0; n2 < cols; ++n2) {
            gradCT[n2][n1] = gradCEval[n1][n2];
        }
    }

    // rTerm = gradCT * deltaLambda
    size_t n = M.size();
    size_t m = deltaLambda.size();
    std::vector<glm::vec3> rTerm(M.size(), glm::vec3(0.0f));
    for (size_t i = 0; i < n; ++i)
    {
        for (size_t j = 0; j < m; ++j)
        {
            // std::cout << "gradCT[" << i << "][" << j << "] = " << glm::to_string(gradCT[i][j])
            // << ", deltaLambda[" << j << "] = " << deltaLambda[j] << std::endl;

            rTerm[i] = gradCT[i][j] * deltaLambda[j];
        }
    }

    // deltaX = M * rTerm
    std::vector<glm::vec3> deltaX(M.size(), glm::vec3(0.0f));
    for (size_t i = 0; i < n; ++i)
    {
        for (size_t j = 0; j < n; ++j)
        {
            deltaX[i] += M[i][j] * rTerm[j];
        }
    }

    return deltaX;
}


void Scene::applyPBD(
    Object& object,
    float deltaTime
)
{
    // for (auto& vertexTransform : object.getVertexTransforms())
    // {
    //     glm::vec3 acceleration = vertexTransform.getAcceleration();
    //     glm::vec3 velocity = vertexTransform.getVelocity();
    //     glm::vec3 position = vertexTransform.getPosition();

    //     velocity += acceleration * deltaTime;
    //     position += velocity * deltaTime;

    //     if (position.y < 0.0f && velocity.y != 0.0f)
    //     {
    //         position.y = 0.0f;
    //         velocity = glm::vec3(0.0f);
    //         acceleration = glm::vec3(0.0f);
    //     }

    //     vertexTransform.setPosition(position);
    //     vertexTransform.setVelocity(velocity);
    //     vertexTransform.setAcceleration(acceleration);
    // }



    std::vector<std::vector<float>> M = object.getMass();
    std::vector<std::function<float(const std::vector<glm::vec3>&)>> C = object.getMesh().lengthConstraints;
    std::vector<std::function<std::vector<glm::vec3>(const std::vector<glm::vec3>&)>> gradC = object.getMesh().gradLengthConstraints;
    float alpha = 0.001f;
    std::vector<float> alphaTilde(C.size(), alpha / (deltaTime * deltaTime));

    std::vector<glm::vec3> position(object.getVertexTransforms().size(), glm::vec3(0.0f));
    std::vector<glm::vec3> velocity(object.getVertexTransforms().size(), glm::vec3(0.0f));
    std::vector<glm::vec3> acceleration(object.getVertexTransforms().size(), glm::vec3(0.0f));
    std::vector<glm::vec3> xTilde(object.getVertexTransforms().size(), glm::vec3(0.0f));

    for (size_t i = 0; i < object.getVertexTransforms().size(); ++i)
    {
        Transform& vertexTransform = object.getVertexTransforms()[i];

        position[i] = vertexTransform.getPosition();
        velocity[i] = vertexTransform.getVelocity();
        acceleration[i] = vertexTransform.getAcceleration();

        glm::vec3 predictedPosition = position[i] +
                                      deltaTime * velocity[i] +
                                      deltaTime * deltaTime * acceleration[i];
        xTilde[i] = predictedPosition;
    }

    // initialize current positions and lambdas
    std::vector<glm::vec3> x_i = xTilde;
    std::vector<float> lambda_i(C.size(), 0.0f);

    // initialize differences of old and new positions and lambdas
    std::vector<float> lambdaDifference(C.size(), 0.0f);
    std::vector<float> positionDifference(x_i.size(), 0.0f);

    const int maxSolverIterations = 1;
    const float convergenceThreshold = 1e-5f; // Threshold for convergence
    int i = 0;

    while (i < maxSolverIterations)
    {
        std::vector<std::vector<glm::vec3>> gradCEval(C.size());
        std::vector<glm::vec3> deltaX(x_i.size(), glm::vec3(0.0f));
        std::vector<float> deltaLambda(lambda_i.size(), 0.0f);

        for (size_t j = 0; j < C.size(); ++j)
        {
            float C_j = C[j](x_i);
            std::vector<glm::vec3> gradC_j = gradC[j](x_i);
            gradCEval[j] = gradC_j;

            float gradCMgradCT = calculateGradCMgradCT(gradC_j, M);

            deltaLambda[j] = (- C_j - alphaTilde[j] * lambda_i[j]) /
                             (gradCMgradCT + alphaTilde[j]);

            // std::cout << "C_j = " << C_j << ", gradCMgradCT = " << gradCMgradCT << '\n';


            // Check if C_j is NaN
            if (std::isnan(C_j))
            {
                std::cerr << "Error: C_j[" << j << "] is NaN!" << std::endl;
                std::exit(EXIT_FAILURE);
            }
            // Check if gradCMgradCT is NaN
            if (std::isnan(gradCMgradCT))
            {
                std::cerr << "Error: gradCMgradCT for constraint [" << j << "] is NaN!" << std::endl;
                std::exit(EXIT_FAILURE);
            }
        }

        deltaX = calculateDeltaX(gradCEval, deltaLambda, M);

        bool lambdaConverged = true;
        bool positionConverged = true;

        for (size_t k = 0; k < lambda_i.size(); ++k)
        {
            float oldLambda = lambda_i[k];
            lambda_i[k] += deltaLambda[k];

            lambdaDifference[k] = std::abs(lambda_i[k] - oldLambda);

            // Check if any lambdaDifference is above the threshold
            if (lambdaDifference[k] >= convergenceThreshold)
            {
                lambdaConverged = false;
            }
        }

        for (size_t k = 0; k < x_i.size(); ++k)
        {
            glm::vec3 oldX = x_i[k];
            x_i[k] += deltaX[k];

            positionDifference[k] = glm::distance(x_i[k], oldX);

            // Check if any positionDifference is above the threshold
            if (positionDifference[k] >= convergenceThreshold)
            {
                positionConverged = false;
            }
        }

        object.setLambdaDifference(lambdaDifference);
        object.setPositionDifference(positionDifference);

        // Check for convergence
        if (lambdaConverged && positionConverged)
        {
            // std::cout << "Converged after " << i + 1 << " iterations." << std::endl;
            break;
        }

        ++i;
    }

    // update position and velocities
    for (size_t n = 0; n < object.getVertexTransforms().size(); ++n)
    {
        Transform& vertexTransform = object.getVertexTransforms()[n];

        glm::vec3 newX = x_i[n];
        glm::vec3 newV = (newX - position[n]) / deltaTime;
        glm::vec3 newA = acceleration[n];

        if (newX.y < 0.0f && newV.y != 0.0f)
        {
            newX.y = 0.0f;
            newV.y = 0.0f;
            newA.y = 0.0f;
        }

        vertexTransform.setPosition(newX);
        vertexTransform.setVelocity(newV);
        vertexTransform.setAcceleration(newA);

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
            // applyPBD(*object, deltaTime);
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