#include "Mesh.hpp"

// #define GLM_ENABLE_EXPERIMENTAL
// #include <glm/gtx/string_cast.hpp>

void Mesh::constructDistanceConstraintVertices(const aiMesh* mesh)
{
    struct UniqueEdge
    {
        unsigned int v1;
        unsigned int v2;
        bool operator==(const UniqueEdge& other) const
        {
            return (v1 == other.v1 && v2 == other.v2) || (v1 == other.v2 && v2 == other.v1);
        }
        bool operator<(const UniqueEdge& other) const
        {
            int a1 = std::min(v1, v2), a2 = std::max(v1, v2);
            int b1 = std::min(other.v1, other.v2), b2 = std::max(other.v1, other.v2);
            return std::tie(a1, a2) < std::tie(b1, b2);
        }
    };

    std::set<UniqueEdge> uniqueEdges;
    for (unsigned int i = 0; i < mesh->mNumFaces; ++i)
    {
        const aiFace& face = mesh->mFaces[i];
        if (face.mNumIndices != 3) continue;

        // Map mesh vertex indices to unique position indices
        unsigned int idx[3];
        for (int j = 0; j < 3; ++j)
        {
            glm::vec3 pos(mesh->mVertices[face.mIndices[j]].x,
                          mesh->mVertices[face.mIndices[j]].y,
                          mesh->mVertices[face.mIndices[j]].z
            );
            auto it = std::find(positions.begin(), positions.end(), pos);
            idx[j] = static_cast<int>(std::distance(positions.begin(), it));
        }

        uniqueEdges.insert(UniqueEdge{idx[0], idx[1]});
        uniqueEdges.insert(UniqueEdge{idx[1], idx[2]});
        uniqueEdges.insert(UniqueEdge{idx[2], idx[0]});
    }

    for (const auto& e : uniqueEdges)
    {
        Edge edge;
        edge.v1 = e.v1;
        edge.v2 = e.v2;
        distanceConstraintVertices.push_back(edge);
    }
}

void Mesh::constructVolumeConstraintVertices(const aiMesh* mesh)
{
    // volumeConstraintVertices.clear();
    for (unsigned int i = 0; i < mesh->mNumFaces; ++i)
    {
        const aiFace& face = mesh->mFaces[i];
        if (face.mNumIndices != 3) continue;

        // For each triangle, find the indices in positions
        unsigned int idx[3];
        for (int j = 0; j < 3; ++j)
        {
            glm::vec3 pos(mesh->mVertices[face.mIndices[j]].x,
                          mesh->mVertices[face.mIndices[j]].y,
                          mesh->mVertices[face.mIndices[j]].z
            );
            auto it = std::find(positions.begin(), positions.end(), pos);
            idx[j] = static_cast<unsigned int>(std::distance(positions.begin(), it));
        }

        Triangle tri;
        tri.v1 = idx[0];
        tri.v2 = idx[1];
        tri.v3 = idx[2];
        volumeConstraintVertices.push_back(tri);
    }
}

void Mesh::constructEnvCollisionConstraintVertices(const aiMesh* mesh)
{
    for (size_t i = 0; i < mesh->mNumVertices; ++i)
    {
        glm::vec3 pos(mesh->mVertices[i].x,
                      mesh->mVertices[i].y,
                      mesh->mVertices[i].z
        );

        auto it = std::find(positions.begin(), positions.end(), pos);
        if (it != positions.end())
        {
            size_t idx = static_cast<unsigned int>(std::distance(positions.begin(), it));
            envCollisionConstraintVertices.push_back(idx);
        }
    }

    //     // Print out envCollisionConstraintVertices
    // std::cout << "envCollisionConstraintVertices: ";
    // for (auto idx : envCollisionConstraintVertices)
    // {
    //     std::cout << idx << " ";
    // }
    // std::cout << std::endl;
}

void Mesh::loadObjData(const std::string& filePath)
{
    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(
        filePath,
        aiProcess_Triangulate  | aiProcess_FlipUVs | aiProcess_GenSmoothNormals
    );

    if (!scene || !scene->HasMeshes())
    {
        std::cerr << "ASSIMP: Failed to load mesh: " << filePath << std::endl;
        return;
    }

    const aiMesh* mesh = scene->mMeshes[0];

    for (size_t i = 0; i < mesh->mNumVertices; ++i)
    {
        Vertex vertex;

        // vertex positions
        glm::vec3 vector;
        vector.x = mesh->mVertices[i].x;
        vector.y = mesh->mVertices[i].y;
        vector.z = mesh->mVertices[i].z;
        vertex.position = vector;

        // TODO : maybe simply refer to the address with pointers and store only unique ones
        // keep track of only unique vertex positions
        auto pit = std::find(positions.begin(), positions.end(), vertex.position);
        if (pit == positions.end())
        {
            positions.push_back(vertex.position);
            m_duplicatePositionIndices.push_back({i});
        }
        else
        {
            size_t pidx = std::distance(positions.begin(), pit);
            m_duplicatePositionIndices[pidx].push_back(i);
        }

        // vertex texture coordinates
        if(mesh->mTextureCoords[0])
        {
            glm::vec2 vec;
            vec.x = mesh->mTextureCoords[0][i].x;
            vec.y = mesh->mTextureCoords[0][i].y;
            vertex.texCoords = vec;
        }
        else
        {
            vertex.texCoords = glm::vec2(0.0f, 0.0f);
        }

        // vertex texture normals
        vector.x = mesh->mNormals[i].x;
        vector.y = mesh->mNormals[i].y;
        vector.z = mesh->mNormals[i].z;
        vertex.normal = vector;

        m_vertices.push_back(vertex);
    }

    // indices
    for(size_t i = 0; i < mesh->mNumFaces; i++)
    {
        aiFace face = mesh->mFaces[i];
        for(unsigned int j = 0; j < face.mNumIndices; j++)
        {
            m_indices.push_back(face.mIndices[j]);
        }
    }

    for (size_t i = 0; i + 2 < m_indices.size(); i += 3)
    {
        unsigned int idx0 = m_indices[i];
        unsigned int idx1 = m_indices[i + 1];
        unsigned int idx2 = m_indices[i + 2];

        const glm::vec3& v0 = m_vertices[idx0].position;
        const glm::vec3& v1 = m_vertices[idx1].position;
        const glm::vec3& v2 = m_vertices[idx2].position;

        glm::vec3 faceNormal = glm::normalize(glm::cross(v1 - v0, v2 - v0));
        faceNormals.push_back(faceNormal);
    }

    // construct vertices used for specific constraints
    constructDistanceConstraintVertices(mesh);
    constructVolumeConstraintVertices(mesh);
    constructEnvCollisionConstraintVertices(mesh);
}

void Mesh::constructDistanceConstraints()
{
    for (const auto& edge : distanceConstraintVertices)
    {
        unsigned int v1 = edge.v1;
        unsigned int v2 = edge.v2;
        float d_0 = glm::distance(positions[v1], positions[v2]);

        distanceConstraints.push_back([=](const std::vector<glm::vec3>& x) -> float {
            return glm::distance(x[v1], x[v2]) - d_0;
        });
    }
}

void Mesh::constructGradDistanceConstraints()
{
    for (const auto& edge : distanceConstraintVertices)
    {
        unsigned int v1 = edge.v1;
        unsigned int v2 = edge.v2;

        gradDistanceConstraints.push_back([=](const std::vector<glm::vec3>& x) -> std::vector<glm::vec3> {
            glm::vec3 n = (x[v1] - x[v2]) / glm::distance(x[v1], x[v2]);
            return { n, -n };
        });
    }
}

void Mesh::constructVolumeConstraints(float& k)
{
    float V_0 = 0.0f;
    float factor = 1.0f / 6.0f;
    for (const auto& triangle : volumeConstraintVertices)
    {
        unsigned int v1 = triangle.v1;
        unsigned int v2 = triangle.v2;
        unsigned int v3 = triangle.v3;
        V_0 += factor * glm::dot(glm::cross(positions[v1], positions[v2]), positions[v3]);
    }

    volumeConstraints.push_back([this, factor, V_0, &k](const std::vector<glm::vec3>& x) -> float {
        float V = 0.0f;
        for (const auto& triangle : volumeConstraintVertices)
        {
            unsigned int v1 = triangle.v1;
            unsigned int v2 = triangle.v2;
            unsigned int v3 = triangle.v3;
            V += factor * glm::dot(glm::cross(x[v1], x[v2]), x[v3]);
        }

        return V - k * V_0;
    });
}

void Mesh::constructGradVolumeConstraints()
{
    float factor = 1.0f / 6.0f;
    for (const auto& triangle : volumeConstraintVertices)
    {
        unsigned int v1 = triangle.v1;
        unsigned int v2 = triangle.v2;
        unsigned int v3 = triangle.v3;
        gradVolumeConstraints.push_back([=](const std::vector<glm::vec3>& x) -> std::vector<glm::vec3> {
            glm::vec3 n1(0.0f), n2(0.0f), n3(0.0f);

            n1 += factor * glm::cross(x[v2], x[v3]);
            n2 += factor * glm::cross(x[v3], x[v1]);
            n3 += factor * glm::cross(x[v1], x[v2]);
            return { n1, n2, n3 };
        });
    }
}

void Mesh::setCandidateContactPlaneNormals(const std::vector<Mesh*>& meshes)
{
    for (const auto* mesh : meshes)
    {
        if (mesh == this) continue; // Skip self

        // Store pointer to the normals vector
        for (const auto& normal : mesh->faceNormals)
        {
            m_candidateNormals.push_back(&normal);
        }
    }
}

void Mesh::constructEnvCollisionConstraints()
{
    for (const auto& v : envCollisionConstraintVertices)
    {
        for (const auto& n : m_candidateNormals)
        {
            envCollisionConstraints.push_back([=, this](const std::vector<glm::vec3>& x) -> float {
                return glm::dot(*n, x[v] - positions[v]);
            });
        }
    }

    // std::cout << "envCollisionConstraints size: " << envCollisionConstraints.size() << std::endl;

}

void Mesh::constructGradEnvCollisionConstraints()
{
    for (const auto& v : envCollisionConstraintVertices)
    {
        for (const auto& n : m_candidateNormals)
        {
            gradEnvCollisionConstraints.push_back([=](const std::vector<glm::vec3>& x) -> std::vector<glm::vec3> {
                return { *n };
            });
        }
    }

    // std::cout << "gradEnvCollisionConstraints size: " << gradEnvCollisionConstraints.size() << std::endl;
}

void Mesh::updateEnvCollisionConstraintVertices()
{
    std::vector<unsigned int> updatedEnvCollisionConstraintVertices;
    for (const auto& v : envCollisionConstraintVertices)
    {
        for (const auto& n : m_candidateNormals)
        {
            updatedEnvCollisionConstraintVertices.push_back(v);
        }
    }

    envCollisionConstraintVertices = std::move(updatedEnvCollisionConstraintVertices);
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
    glBufferData(GL_ARRAY_BUFFER, m_vertices.size() * sizeof(Vertex), &m_vertices[0], GL_DYNAMIC_DRAW);

    // Bind and set EBO
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_indices.size() * sizeof(unsigned int), &m_indices[0], GL_STATIC_DRAW);

    // Position attribute
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);

    // Texture coordinate attribute
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texCoords));

    // Normal coordinate attribute
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));

    glBindVertexArray(0);

}

void Mesh::update()
{
    size_t n = positions.size();
    for (size_t i = 0; i < n; ++i)
    {
        const glm::vec3& updatedPosition = positions[i];
        const auto& duplicates = m_duplicatePositionIndices[i];
        for (unsigned int idx : duplicates)
        {
            m_vertices[idx].position = updatedPosition;
        }
    }

    for (size_t i = 0, tri = 0; i + 2 < m_indices.size(); i += 3, ++tri)
    {
        unsigned int idx0 = m_indices[i];
        unsigned int idx1 = m_indices[i + 1];
        unsigned int idx2 = m_indices[i + 2];

        glm::vec3& v0 = m_vertices[idx0].position;
        glm::vec3& v1 = m_vertices[idx1].position;
        glm::vec3& v2 = m_vertices[idx2].position;

        glm::vec3 faceNormal = glm::normalize(glm::cross(v1 - v0, v2 - v0));

        m_vertices[idx0].normal = faceNormal;
        m_vertices[idx1].normal = faceNormal;
        m_vertices[idx2].normal = faceNormal;

        faceNormals[tri] = faceNormal;
    }


    // // Print m_candidateNormals
    // std::cout << "m_candidateNormals (" << m_candidateNormals.size() << "):" << std::endl;
    // for (size_t i = 0; i < m_candidateNormals.size(); ++i)
    // {
    //     const glm::vec3* n = m_candidateNormals[i];
    //     if (n)
    //     {
    //         std::cout << "  [" << i << "]: (" << n->x << ", " << n->y << ", " << n->z << ")" << std::endl;
    //     }
    // }
}

void Mesh::draw()
{
    glBindVertexArray(m_VAO);
    glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
    glBufferSubData(GL_ARRAY_BUFFER, 0, m_vertices.size() * sizeof(Vertex), &m_vertices[0]);

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