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
#include <map>


#include "Transform.hpp"
#include "Shader.hpp"

class Object; // Forward declaration


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

// struct TriangleFaceNormal
// {
//     Triangle triangle;
//     glm::vec3 faceNormal;
// };


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
    void drawVertexNormals();
    void drawFaceNormals();
    void destroy();

    // void setCandidateMeshes(const std::vector<Mesh*>& meshes);
    void setCandidateObjectMeshes(const std::vector<Object*>& objects);

    void constructDistanceConstraints();
    void constructVolumeConstraints(float& k);
    void constructEnvCollisionConstraints();

    // void constructTriangleFaceNormals();

public:
    std::vector<glm::vec3>& getPositions() { return m_positions; }
    const std::vector<Vertex>& getVertices() const { return m_vertices; }

    struct DistanceConstraints
    {
        std::vector<Edge> edges;
        std::vector<std::function<float(const std::vector<glm::vec3>&)>> C;
        std::vector<std::function<std::vector<glm::vec3>(const std::vector<glm::vec3>&)>> gradC;
    };
    DistanceConstraints distanceConstraints;

    struct VolumeConstraints
    {
        std::vector<Triangle> triangles;
        std::vector<std::function<float(const std::vector<glm::vec3>&)>> C;
        std::vector<std::function<std::vector<glm::vec3>(const std::vector<glm::vec3>&)>> gradC;
    };
    VolumeConstraints volumeConstraints;

    std::vector<unsigned int> envCollisionConstraintVertices;
    struct EnvCollisionConstraints
    {
        std::vector<unsigned int> vertices;
        const Mesh* candidateMesh;
        std::vector<std::function<float(const std::vector<glm::vec3>&)>> C;
        std::vector<std::function<std::vector<glm::vec3>(const std::vector<glm::vec3>&)>> gradC;

        std::map<unsigned int, std::vector<size_t>> vertexToConstraints;
    };
    std::vector<EnvCollisionConstraints> perEnvCollisionConstraints;

private:
    void loadObjData(const std::string& meshPath);

    void initVerticesBuffer();
    void initVertexNormalsBuffer();
    void initFaceNormalsBuffer();

    void constructVertices(const aiMesh* mesh);
    void constructIndices(const aiMesh* mesh);

    std::vector<Triangle> constructTriangles();
    std::vector<glm::vec3> calculateFaceNormals();

    // void constructVertexNormals();
    // void constructFaceNormals();

    void constructDistanceConstraintVertices();
    void constructVolumeConstraintVertices();
    void constructEnvCollisionConstraintVertices();

private:
    std::string m_name;
    std::string m_meshPath;

    std::vector<glm::vec3> m_positions;
    std::vector<std::vector<unsigned int>> m_duplicatePositionIndices;

    GLuint m_VAO, m_VBO, m_EBO;
    std::vector<Vertex> m_vertices;
    std::vector<unsigned int> m_indices;

    GLuint m_vertexNormalVAO, m_vertexNormalVBO;
    GLuint m_faceNormalVAO, m_faceNormalVBO;
    // std::vector<glm::vec3> m_vertexNormals;
    // std::vector<glm::vec3> m_faceNormals;
    float m_vertexNormalLength;
    float m_faceNormalLength;

    std::vector<const Mesh*> m_candidateObjectMeshes;
    // std::vector<TriangleFaceNormal> m_triangleFaceNormals;
};