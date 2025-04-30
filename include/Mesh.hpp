#pragma once

#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <iostream>
#include <glad.h>

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
    const float getSideLength()     const { return m_sideLength; }

    void draw();
    void deleteMesh();

private:
    void getVertexData(const std::string& meshPath);
    void getVertexCount();

    std::string m_name;
    std::string m_meshPath;
    float m_sideLength;

    GLuint m_VAO, m_VBO;

    std::vector<float> vertices;
    std::vector<float> positions;
    std::vector<float> texCoords;
    unsigned int m_vertexCount;
};