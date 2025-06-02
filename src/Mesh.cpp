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

    std::set<std::pair<int, int>> uniqueEdges; // To avoid duplicate edges

    std::string line;
    while (std::getline(file, line))
    {
        std::istringstream iss(line);
        std::string prefix;
        iss >> prefix;

        if (prefix == "v") {
            float x, y, z;
            iss >> x >> y >> z;
            positions.emplace_back(x, y, z);
            continue;
        }
        if (prefix == "vt") {
            float u, v;
            iss >> u >> v;
            texCoords.emplace_back(u, v);
            continue;
        }
        if (prefix == "vn") {
            float nx, ny, nz;
            iss >> nx >> ny >> nz;
            normals.emplace_back(nx, ny, nz);
            continue;
        }
        if (prefix == "f") {
            std::array<unsigned int, 3> vertexIndices;
            std::array<unsigned int, 3> texCoordIndices;
            std::array<unsigned int, 3> normalIndices;

            for (int i = 0; i < 3; ++i)
            {
                std::string vertexData;
                iss >> vertexData;
                std::istringstream vertexStream(vertexData);
                std::string v, vt, vn;
                std::getline(vertexStream, v, '/');
                std::getline(vertexStream, vt, '/');
                std::getline(vertexStream, vn, '/');

                vertexIndices[i] = std::stoi(v) - 1;
                texCoordIndices[i] = std::stoi(vt) - 1;
                normalIndices[i] = std::stoi(vn) - 1;

                // Add position, texture, and normal to m_vertices
                m_vertices.push_back(positions[vertexIndices[i]].x);
                m_vertices.push_back(positions[vertexIndices[i]].y);
                m_vertices.push_back(positions[vertexIndices[i]].z);

                m_vertices.push_back(texCoords[texCoordIndices[i]].x);
                m_vertices.push_back(texCoords[texCoordIndices[i]].y);

                m_vertices.push_back(normals[normalIndices[i]].x);
                m_vertices.push_back(normals[normalIndices[i]].y);
                m_vertices.push_back(normals[normalIndices[i]].z);

                m_positionMapping.push_back(vertexIndices[i]);
                indices.push_back(static_cast<unsigned int>(indices.size()));
            }

            // Store the triangle
            volumeConstraintVertices.push_back({static_cast<int>(vertexIndices[0]),
                                   static_cast<int>(vertexIndices[1]),
                                   static_cast<int>(vertexIndices[2])});


            for (int i = 0; i < 3; ++i)
            {
                int v1 = vertexIndices[i];
                int v2 = vertexIndices[(i + 1) % 3];

                auto edge = std::minmax(v1, v2);
                uniqueEdges.insert(edge);
            }
        }
    }

    distanceConstraintVertices.clear();
    for (const auto& edge : uniqueEdges)
    {
        distanceConstraintVertices.push_back({edge.first, edge.second});
    }

    file.close();
    m_indices = indices;
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

void Mesh::constructVolumeConstraints()
{
    float V_0 = 0.0f;
    for (const auto& vertexTriple : volumeConstraintVertices)
    {
        int v1 = vertexTriple[0];
        int v2 = vertexTriple[1];
        int v3 = vertexTriple[2];
        V_0 += (1.0f / 6.0f) * glm::dot(glm::cross(positions[v1], positions[v2]), positions[v3]);
    }

    volumeConstraints.push_back([this, V_0](const std::vector<glm::vec3>& x) -> float {
        float V = 0.0f;

        for (const auto& vertexTriple : volumeConstraintVertices)
        {
            int v1 = vertexTriple[0];
            int v2 = vertexTriple[1];
            int v3 = vertexTriple[2];
            V += (1.0f / 6.0f) * glm::dot(glm::cross(x[v1], x[v2]), x[v3]);
        }

        return V - V_0;
    });


}

void Mesh::constructGradVolumeConstraints()
{
    gradVolumeConstraints.push_back([this](const std::vector<glm::vec3>& x) -> std::vector<glm::vec3> {
        glm::vec3 n1(0.0f), n2(0.0f), n3(0.0f);

        for (const auto& vertexTriple : volumeConstraintVertices)
        {
            int v1 = vertexTriple[0];
            int v2 = vertexTriple[1];
            int v3 = vertexTriple[2];

            n1 += (1.0f / 6.0f) * glm::cross(x[v2], x[v3]);
            n2 += (1.0f / 6.0f) * glm::cross(x[v3], x[v1]);
            n3 += (1.0f / 6.0f) * glm::cross(x[v1], x[v2]);
        }

        return { n1, n2, n3 };
    });
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