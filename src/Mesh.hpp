#pragma once

#include <string>
#include <vector>
#include <functional>
#include <fstream>
#include <sstream>
#include <iostream>
#include <glad.h>
#include <glm/glm.hpp>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <set>

#include "Transform.hpp"
#include "Shader.hpp"

struct Vertex
{
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec2 texCoords;
};

struct Edge
{
    unsigned int v1;
    unsigned int v2;
};

struct Triangle
{
    unsigned int v1;
    unsigned int v2;
    unsigned int v3;
};


class Mesh
{
public:
    Mesh() = default;
    Mesh(
        const std::string& name,
        const std::string& meshPath
    );

    const std::string getName()     const { return m_name; }
    const std::string getMeshPath() const { return m_meshPath; }

    void update();
    void draw();
    void deleteMesh();

    void setCandidateContactPlaneNormals(const std::vector<Mesh*>& meshes);

    void constructDistanceConstraints();
    void constructGradDistanceConstraints();

    void constructVolumeConstraints();
    void constructGradVolumeConstraints();

    void constructEnvCollisionConstraints();
    void constructGradEnvCollisionConstraints();

public:
    std::vector<glm::vec3> positions;
    std::vector<glm::vec3> faceNormals;

    void setNormalShader(Shader normalShader) { m_normalShader = std::move(normalShader); };

    std::vector<Edge> distanceConstraintVertices;
    std::vector<std::function<float(const std::vector<glm::vec3>&)>> distanceConstraints;
    std::vector<std::function<std::vector<glm::vec3>(const std::vector<glm::vec3>&)>> gradDistanceConstraints;

    std::vector<Triangle> volumeConstraintVertices;
    std::vector<std::function<float(const std::vector<glm::vec3>&)>> volumeConstraints;
    std::vector<std::function<std::vector<glm::vec3>(const std::vector<glm::vec3>&)>> gradVolumeConstraints;
    void updateVolumeConstraintVertices();

    std::vector<unsigned int> envCollisionConstraintVertices;
    std::vector<std::function<float(const std::vector<glm::vec3>&)>> envCollisionConstraints;
    std::vector<std::function<std::vector<glm::vec3>(const std::vector<glm::vec3>&)>> gradEnvCollisionConstraints;
    void updateEnvCollisionConstraintVertices();

private:
    void loadObjData(const std::string& meshPath);
    void constructDistanceConstraintVertices(const aiMesh* mesh);
    void constructVolumeConstraintVertices(const aiMesh* mesh);
    void constructEnvCollisionConstraintVertices(const aiMesh* mesh);

private:
    std::string m_name;
    std::string m_meshPath;

    std::vector<std::vector<size_t>> m_duplicatePositionIndices;

    GLuint m_VAO, m_VBO, m_EBO;
    std::vector<Vertex> m_vertices;
    std::vector<unsigned int> m_indices;

    Shader m_normalShader;
    // std::vector<const std::vector<glm::vec3>*> m_candidateNormals;
    std::vector<const glm::vec3*> m_candidateNormals;
};