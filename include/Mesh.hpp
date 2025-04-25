#pragma once

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <glad.h>

class Mesh
{
public:
    Mesh() = default;
    Mesh(const std::string& name, const std::string& meshPath);

    const std::string getName()     const { return m_name; }
    const std::string getMeshPath() const { return m_meshPath; }

    void draw();
    void deleteMesh();

private:
    std::string m_name;
    std::string m_meshPath;

    unsigned int m_VAO, m_VBO;
    unsigned int m_vertexCount;
};