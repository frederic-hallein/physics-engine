#include "Mesh.hpp"

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/string_cast.hpp>

void Mesh::loadObjData(const std::string& filePath)
{

    std::vector<unsigned int> indices;

    std::ifstream file(filePath);
    if (!file.is_open())
    {
        std::cerr << "ERROR::MESH::FILE_NOT_SUCCESSFULLY_OPENED: " << filePath << std::endl;
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
            positions.emplace_back(x, y, z);
        }
        else if (prefix == "vt") // Texture coordinate
        {
            float u, v;
            iss >> u >> v;
            texCoords.emplace_back(u, v);
        }
        else if (prefix == "vn") // Normal vector
        {
            float nx, ny, nz;
            iss >> nx >> ny >> nz;
            normals.emplace_back(nx, ny, nz);
        }
        else if (prefix == "f") // Face data
        {
            std::string vertexData;
            unsigned int vertexIndices[3];
            for (int i = 0; i < 3; ++i)
            {
                iss >> vertexData;

                // Parse the vertex/texture/normal indices
                std::istringstream vertexStream(vertexData);
                std::string v, vt, vn;
                std::getline(vertexStream, v, '/');
                std::getline(vertexStream, vt, '/');
                std::getline(vertexStream, vn, '/');

                unsigned int vertexIndex = std::stoi(v) - 1;
                vertexIndices[i] = vertexIndex;

                unsigned int texCoordIndex = std::stoi(vt) - 1;
                unsigned int normalIndex = std::stoi(vn) - 1;

                // Add position, texture, and normal to m_vertices
                m_vertices.push_back(positions[vertexIndex].x);
                m_vertices.push_back(positions[vertexIndex].y);
                m_vertices.push_back(positions[vertexIndex].z);

                m_vertices.push_back(texCoords[texCoordIndex].x);
                m_vertices.push_back(texCoords[texCoordIndex].y);

                m_vertices.push_back(normals[normalIndex].x);
                m_vertices.push_back(normals[normalIndex].y);
                m_vertices.push_back(normals[normalIndex].z);

                m_positionMapping.push_back(vertexIndex);

                indices.push_back(static_cast<unsigned int>(indices.size()));
            }

            // keep track of adjacent vertices which will be used for constraints
            for (int i = 0; i < 3; ++i)
            {
                int v1 = vertexIndices[i];
                int v2 = vertexIndices[(i + 1) % 3];
                int v3 = vertexIndices[(i + 2) % 3];

                m_connectedVertices[v1].insert(v2);
                m_connectedVertices[v1].insert(v3);

                m_connectedVertices[v2].insert(v1);
                m_connectedVertices[v2].insert(v3);

                m_connectedVertices[v3].insert(v1);
                m_connectedVertices[v3].insert(v2);
            }
        }
    }

    file.close();
    m_indices = indices;
}

void Mesh::setDistanceConstraintVertices()
{
    for (const auto& [vertex, neighbors] : m_connectedVertices)
    {
        for (int neighbor : neighbors)
        {
            if (vertex < neighbor) // select unique
            {
                distanceConstraintVertices.push_back({ vertex , neighbor });
            }
        }
    }


    // for (const auto& [vertex1, neighbors] : m_vertexDistances)
    // {
    //     for (const auto& [vertex2, distance] : neighbors)
    //     {
    //         std::cout << "(" << vertex1 << ", " << vertex2
    //                   << "): " << distance << std::endl;
    //     }
    // }
}

void Mesh::constructDistanceConstraints()
{
    for (const auto& vertexPair : distanceConstraintVertices)
    {
        int v1 = vertexPair[0];
        int v2 = vertexPair[1];
        float d_0 = glm::distance(positions[v1], positions[v2]);

        distanceConstraints.push_back([=](const std::vector<glm::vec3>& x) -> float {
            return glm::distance(x[v1], x[v2]) - d_0;
        });
    }
}

void Mesh::constructGradDistanceConstraints()
{
    for (const auto& vertexPair : distanceConstraintVertices)
    {
        int v1 = vertexPair[0];
        int v2 = vertexPair[1];
        gradDistanceConstraints.push_back([this, v1, v2](const std::vector<glm::vec3>& x) -> std::vector<glm::vec3> {
            glm::vec3 n = (x[v1] - x[v2]) / (glm::distance(x[v1], x[v2]));
            return { n, -n };
        });

    }
}

void Mesh::setVolumeConstraintVertices()
{
    // TODO : obtain all tetraeders
}

void Mesh::constructVolumeConstraints()
{
    for (const auto& vertexQuad : volumeConstraintVertices)
    {
        int v1 = vertexQuad[0];
        int v2 = vertexQuad[1];
        int v3 = vertexQuad[2];
        int v4 = vertexQuad[3];
        float V_0 = 0.0f; // TODO : calculate rest volume

        volumeConstraints.push_back([=](const std::vector<glm::vec3>& x) -> float {
            return glm::dot(glm::cross(x[v2] - x[v1], x[v3] - x[v1]), x[v4] - x[v1]) - 6 * V_0;
        });
    }
}

void Mesh::constructGradVolumeConstraints()
{
    for (const auto& vertexQuad : volumeConstraintVertices)
    {
        int v1 = vertexQuad[0];
        int v2 = vertexQuad[1];
        int v3 = vertexQuad[2];
        int v4 = vertexQuad[3];
        gradVolumeConstraints.push_back([this, v1, v2, v3, v4](const std::vector<glm::vec3>& x) -> std::vector<glm::vec3> {
            return
            {
                glm::cross(x[v4] - x[v2], x[v3] - x[v2]),
                glm::cross(x[v3] - x[v1], x[v4] - x[v1]),
                glm::cross(x[v4] - x[v1], x[v2] - x[v1]),
                glm::cross(x[v2] - x[v1], x[v3] - x[v1])
            };
        });
    }
}

Mesh::Mesh(const std::string& name, const std::string& meshPath)
    : m_name(name),
      m_meshPath(meshPath)
{
    loadObjData(meshPath);

    glGenVertexArrays(1, &m_VAO);
    glGenBuffers(1, &m_VBO);
    glGenBuffers(1, &m_EBO);

    glBindVertexArray(m_VAO);

    // Bind and set VBO
    glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
    glBufferData(GL_ARRAY_BUFFER, m_vertices.size() * sizeof(float), m_vertices.data(), GL_STATIC_DRAW);

    // Bind and set EBO
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_indices.size() * sizeof(unsigned int), m_indices.data(), GL_STATIC_DRAW);

    // Position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // Texture coordinate attribute
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // Normal coordinate attribute
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(5 * sizeof(float)));
    glEnableVertexAttribArray(2);
}

void Mesh::update()
{
    for (size_t i = 0; i < m_positionMapping.size(); ++i)
    {
        size_t positionIndex = m_positionMapping[i];
        const glm::vec3& updatedPosition = positions[positionIndex];

        m_vertices[i * 8]     = updatedPosition.x;
        m_vertices[i * 8 + 1] = updatedPosition.y;
        m_vertices[i * 8 + 2] = updatedPosition.z;
    }
}

void Mesh::draw()
{
    glBindVertexArray(m_VAO);
    glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
    glBufferSubData(GL_ARRAY_BUFFER, 0, m_vertices.size() * sizeof(float), m_vertices.data());

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
    glDrawElements(GL_TRIANGLES, m_indices.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

void Mesh::deleteMesh()
{
    glDeleteVertexArrays(1, &m_VAO);
    glDeleteBuffers(1, &m_VBO);
    glDeleteBuffers(1, &m_EBO);
}