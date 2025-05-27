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


#include <set>
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

    void calculateVertexDistances();
    void constructLengthConstraints();
    void constructGradLengthConstraints();
    void constructGradLengthConstraintsTransposed();

public:
    std::vector<glm::vec3> positions;
    std::vector<glm::vec2> texCoords;
    std::vector<glm::vec3> normals;

    std::vector<std::function<float(const std::vector<glm::vec3>&)>> lengthConstraints;
    std::vector<std::function<std::vector<glm::vec3>(const std::vector<glm::vec3>&)>> gradLengthConstraints;

private:
    void loadObjData(const std::string& meshPath);

private:
    std::string m_name;
    std::string m_meshPath;

    std::vector<size_t> m_positionMapping;

    std::vector<float> m_vertices;
    std::vector<unsigned int> m_indices;

    std::unordered_map<int, std::set<int>> m_connectedVertices;
    std::unordered_map<int, std::unordered_map<int, float>> m_vertexDistances;

    GLuint m_VAO, m_VBO, m_EBO;
};