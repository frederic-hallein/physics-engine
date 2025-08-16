#pragma once

#include <string>
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
// #include <map>

#include "ShaderManager.hpp"
#include "MeshManager.hpp"
#include "TextureManager.hpp"
#include "Camera.hpp"
#include "Object.hpp"

#include <span>
#include <omp.h>


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

    Camera* getCamera() { return m_camera.get(); }
    const std::vector<std::unique_ptr<Object>>& getObjects() const { return m_objects; }

    bool& enableDistanceConstraints() { return m_enableDistanceConstraints; }
    void solveDistanceConstraints(
        std::vector<glm::vec3>& x,
        const std::vector<glm::vec3>& posDiff,
        const std::vector<float>& M,
        float alphaTilde,
        float gamma,
        const Mesh::DistanceConstraints& distanceConstraints
    );

    bool& enableVolumeConstraints() { return m_enableVolumeConstraints; }
    void solveVolumeConstraints(
        std::vector<glm::vec3>& x,
        const std::vector<glm::vec3>& posDiff,
        const std::vector<float>& M,
        float alphaTilde,
        float gamma,
        const Mesh::VolumeConstraints& volumeConstraints
    );

    bool& enableEnvCollisionConstraints() { return m_enableEnvCollisionConstraints; }
    void solveEnvCollisionConstraints(
        std::vector<glm::vec3>& x,
        const std::vector<glm::vec3>& posDiff,
        const std::vector<float>& M,
        float alphaTilde,
        float gamma,
        std::vector<Mesh::EnvCollisionConstraints> perEnvCollisionConstraints
    );

    glm::vec3& getGravitationalAcceleration() { return m_gravitationalAcceleration; }
    int& getPBDSubsteps() { return m_pbdSubsteps; }
    float& getAlpha() { return m_alpha; }
    float& getBeta()  { return m_beta;  }
    float& getOverpressureFactor() { return m_k; }

private:
    void createObjects();
    void setupEnvCollisionConstraints();
    void applyGravity(
        Object& object,
        float deltaTime
    );
    float calculateDeltaLambda(
        float C_j,
        const std::vector<glm::vec3>& gradC_j,
        const std::vector<glm::vec3>& posDiff,
        std::span<const unsigned int> constraintVertices,
        const std::vector<float>& M,
        float alphaTilde,
        float gamma
    );
    std::vector<glm::vec3> calculateDeltaX(
        float lambda,
        const std::vector<float>& M,
        std::vector<glm::vec3>& gradC_j,
        std::span<const unsigned int> constraintVertices
    );
    void applyPBD(
        Object& object,
        float deltaTime
    );

private:
    std::string m_name;

    std::unique_ptr<ShaderManager> m_shaderManager;
    std::unique_ptr<MeshManager> m_meshManager;
    std::unique_ptr<TextureManager> m_textureManager;

    std::unique_ptr<Camera> m_camera;

    std::vector<std::unique_ptr<Object>> m_objects;
    // std::vector<Mesh*> m_meshPtrs;

    glm::vec3 m_gravitationalAcceleration;

    int m_pbdSubsteps;

    bool m_enableDistanceConstraints;
    bool m_enableVolumeConstraints;
    bool m_enableEnvCollisionConstraints;

    float m_alpha;
    float m_beta;
    float m_k;


    std::vector<Mesh::EnvCollisionConstraints> m_envCollisionConstraints;
};