#pragma once

#include <string>
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "ShaderManager.hpp"
#include "MeshManager.hpp"
#include "TextureManager.hpp"
#include "Camera.hpp"
#include "Object.hpp"

#include <cmath>
#include <cstdlib>
// #include <omp.h>

class Scene
{
public:
    Scene(
        const std::string& name,
        std::unique_ptr<ShaderManager> shaderManager,
        std::unique_ptr<MeshManager> meshManager,
        std::unique_ptr<TextureManager> textureManager,
        std::unique_ptr<Camera>
    );

    void update(float deltaTime);
    void render();
    void clear();

    glm::vec3 getCameraPosition() const { return m_camera->getPosition(); }
    const std::vector<std::unique_ptr<Object>>& getObjects() const { return m_objects; }

    glm::vec3& getGravitationalAcceleration() { return m_gravitationalAcceleration; }
    float& getAlpha() { return alpha; }

private:
    void applyGravity(
        Object& object,
        float deltaTime
    );
    float calculateDeltaLambda(
        float C_j,
        const std::vector<glm::vec3>& gradC_j,
        const std::vector<glm::vec3>& posDiff,
        const std::vector<unsigned int>& constraintVertices,
        const std::vector<float>& M,
        float alphaTilde,
        float gamma
    );
    std::vector<glm::vec3> calculateDeltaX(
        float lambda,
        const std::vector<float>& M,
        std::vector<glm::vec3>& gradC_j,
        const std::vector<unsigned int>& constraintVertices
    );
    void applyPBD(
        Object& object,
        float deltaTime
    );

    std::string m_name;

    std::unique_ptr<ShaderManager> m_shaderManager;
    std::unique_ptr<MeshManager> m_meshManager;
    std::unique_ptr<TextureManager> m_textureManager;

    std::unique_ptr<Camera> m_camera;

    std::vector<std::unique_ptr<Object>> m_objects;

    glm::vec3 m_gravitationalAcceleration;
    float alpha;


};