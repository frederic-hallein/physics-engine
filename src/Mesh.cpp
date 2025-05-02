#include <string>

#include "Mesh.hpp"

void Mesh::setVertexData(const std::string& meshPath)
{
    std::ifstream file(meshPath);
    if (!file.is_open())
    {
        std::cerr << "ERROR::MESH::FILE_NOT_SUCCESSFULLY_OPENED: " << meshPath << std::endl;
        return;
    }

    std::string line;
    while (std::getline(file, line))
    {
        std::istringstream iss(line);
        std::string prefix;
        iss >> prefix;

        if (prefix == "v") // Vertex position
        {
            float x, y, z;
            iss >> x >> y >> z;
            positions.push_back(x);
            positions.push_back(y);
            positions.push_back(z);
        }
        else if (prefix == "vt") // Texture coordinate
        {
            float u, v;
            iss >> u >> v;
            texCoords.push_back(u);
            texCoords.push_back(v);
        }
    }
    file.close();
}

void Mesh::calculateVertexCount()
{
    size_t vertexCount = std::min(positions.size() / 3, texCoords.size() / 2);
    for (size_t i = 0; i < vertexCount; ++i)
    {
        vertices.push_back(positions[i * 3 + 0]); // x
        vertices.push_back(positions[i * 3 + 1]); // y
        vertices.push_back(positions[i * 3 + 2]); // z
        vertices.push_back(texCoords[i * 2 + 0]); // u
        vertices.push_back(texCoords[i * 2 + 1]); // v
    }

    m_vertexCount = static_cast<int>(vertexCount);
}

Mesh::Mesh(const std::string& name, const std::string& meshPath)
    : m_name(name),
      m_meshPath(meshPath),
      m_sideLength(1.0f) // TODO : get sideLength from file
{
    setVertexData(meshPath);
    calculateVertexCount();

    glGenVertexArrays(1, &m_VAO);
    glGenBuffers(1, &m_VBO);

    glBindVertexArray(m_VAO);

    glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // texture coord attribute
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
}

void Mesh::draw()
{
    glBindVertexArray(m_VAO);
    glDrawArrays(GL_TRIANGLES, 0, m_vertexCount);
}

void Mesh::deleteMesh()
{
    glDeleteVertexArrays(1, &m_VAO);
    glDeleteBuffers(1, &m_VBO);
}