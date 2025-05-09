#pragma once

#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <iostream>
#include <glad.h>
#include <glm/glm.hpp>

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

    // std::vector<glm::vec3>& getPositions() { return m_positions; };
    // const std::vector<glm::vec2>& getTexCoords() const { return m_texCoords; };
    // const std::vector<glm::vec3>& getNormals()   const { return m_normals; };

    const std::vector<float>& getVertices() const { return m_vertices; }

    void update();
    void draw();
    void deleteMesh();

private:
    void loadObjData(const std::string& meshPath);

    std::string m_name;
    std::string m_meshPath;

    // std::vector<glm::vec3> m_positions;
    // std::vector<glm::vec2> m_texCoords;
    // std::vector<glm::vec3> m_normals;

    std::vector<float> m_vertices;
    std::vector<unsigned int> m_indices;

    GLuint m_VAO, m_VBO, m_EBO;
    int m_vertexCount;
};