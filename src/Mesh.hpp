#pragma once

#include <string>
#include <vector>
#include <functional>
#include <fstream>
#include <sstream>
#include <iostream>
#include <glad.h>
#include <glm/glm.hpp>

#include "Transform.hpp"

struct Vertex
{
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec2 texCoords;
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

    void constructDistanceConstraints();
    void constructGradDistanceConstraints();

    void constructVolumeConstraints();
    void constructGradVolumeConstraints();

public:
    std::vector<glm::vec3> positions;
    // std::vector<glm::vec2> texCoords;
    // std::vector<glm::vec3> normals;

    std::vector<std::vector<int>> distanceConstraintVertices;
    std::vector<std::function<float(const std::vector<glm::vec3>&)>> distanceConstraints;
    std::vector<std::function<std::vector<glm::vec3>(const std::vector<glm::vec3>&)>> gradDistanceConstraints;

    std::vector<std::vector<int>> volumeConstraintVertices;
    std::vector<std::function<float(const std::vector<glm::vec3>&)>> volumeConstraints;
    std::vector<std::function<std::vector<glm::vec3>(const std::vector<glm::vec3>&)>> gradVolumeConstraints;
private:
    void loadObjData(const std::string& meshPath);

private:
    std::string m_name;
    std::string m_meshPath;

    std::vector<unsigned int> m_positionMapping;

    std::vector<Vertex> m_vertices;
    std::vector<unsigned int> m_indices;

    GLuint m_VAO, m_VBO, m_EBO;
};