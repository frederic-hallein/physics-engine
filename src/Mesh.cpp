#include "Mesh.hpp"


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
            auto it = std::find(m_positions.begin(), m_positions.end(), pos);
            idx[j] = static_cast<int>(std::distance(m_positions.begin(), it));
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
        distanceConstraints.edges.push_back(edge);
    }
}

void Mesh::constructVolumeConstraintVertices(const aiMesh* mesh)
{
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
            auto it = std::find(m_positions.begin(), m_positions.end(), pos);
            idx[j] = static_cast<unsigned int>(std::distance(m_positions.begin(), it));
        }

        Triangle tri;
        tri.v1 = idx[0];
        tri.v2 = idx[1];
        tri.v3 = idx[2];
        volumeConstraints.triangles.push_back(tri);
    }
}

void Mesh::constructEnvCollisionConstraintVertices(const aiMesh* mesh)
{
    std::set<size_t> uniqueIndices;
    for (size_t i = 0; i < mesh->mNumVertices; ++i)
    {
        glm::vec3 pos(mesh->mVertices[i].x,
                      mesh->mVertices[i].y,
                      mesh->mVertices[i].z
        );

        auto it = std::find(m_positions.begin(), m_positions.end(), pos);
        if (it != m_positions.end())
        {
            size_t idx = static_cast<size_t>(std::distance(m_positions.begin(), it));
            if (uniqueIndices.insert(idx).second) // Only insert if not already present
            {
                envCollisionConstraintVertices.push_back(idx);
            }
        }
    }
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

    // construct m_vertices
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
        auto pit = std::find(m_positions.begin(), m_positions.end(), vertex.position);
        if (pit == m_positions.end())
        {
            m_positions.push_back(vertex.position);
            m_duplicatePositionIndices.push_back({static_cast<unsigned int>(i)});
        }
        else
        {
            size_t pidx = std::distance(m_positions.begin(), pit);
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

        // vertex normals
        vector.x = mesh->mNormals[i].x;
        vector.y = mesh->mNormals[i].y;
        vector.z = mesh->mNormals[i].z;
        vertex.normal = vector;

        m_vertexNormals.push_back(vertex.normal);

        m_vertices.push_back(vertex);
    }

    // construct m_indices
    for(size_t i = 0; i < mesh->mNumFaces; i++)
    {
        aiFace face = mesh->mFaces[i];
        for(unsigned int j = 0; j < face.mNumIndices; j++)
        {
            m_indices.push_back(face.mIndices[j]);
        }
    }

    // construct vertex and face normals
    for (size_t i = 0; i + 2 < m_indices.size(); i += 3)
    {
        unsigned int idx0 = m_indices[i];
        unsigned int idx1 = m_indices[i + 1];
        unsigned int idx2 = m_indices[i + 2];

        const glm::vec3& v0 = m_vertices[idx0].position;
        const glm::vec3& v1 = m_vertices[idx1].position;
        const glm::vec3& v2 = m_vertices[idx2].position;

        glm::vec3 faceNormal = glm::normalize(glm::cross(v1 - v0, v2 - v0));
        m_faceNormals.push_back(faceNormal);

    }




    // construct vertices used for specific constraints
    constructDistanceConstraintVertices(mesh);
    constructVolumeConstraintVertices(mesh);
    constructEnvCollisionConstraintVertices(mesh);
}

void Mesh::constructDistanceConstraints()
{
    for (const auto& edge : distanceConstraints.edges)
    {
        unsigned int v1 = edge.v1;
        unsigned int v2 = edge.v2;
        float d_0 = glm::distance(m_positions[v1], m_positions[v2]);

        distanceConstraints.C.push_back([=](const std::vector<glm::vec3>& x) -> float {
            return glm::distance(x[v1], x[v2]) - d_0;
        });

        distanceConstraints.gradC.push_back([=](const std::vector<glm::vec3>& x) -> std::vector<glm::vec3> {
            glm::vec3 n = (x[v1] - x[v2]) / glm::distance(x[v1], x[v2]);
            return { n, -n };
        });
    }
}

void Mesh::constructVolumeConstraints(float& k)
{
    float V_0 = 0.0f;
    float factor = 1.0f / 6.0f;
    for (const auto& triangle : volumeConstraints.triangles)
    {
        unsigned int v1 = triangle.v1;
        unsigned int v2 = triangle.v2;
        unsigned int v3 = triangle.v3;
        V_0 += factor * glm::dot(glm::cross(m_positions[v1], m_positions[v2]), m_positions[v3]);
    }

    volumeConstraints.C.push_back([this, factor, V_0, &k](const std::vector<glm::vec3>& x) -> float {
        float V = 0.0f;
        for (const auto& triangle : volumeConstraints.triangles)
        {
            unsigned int v1 = triangle.v1;
            unsigned int v2 = triangle.v2;
            unsigned int v3 = triangle.v3;
            V += factor * glm::dot(glm::cross(x[v1], x[v2]), x[v3]);
        }
        return V - k * V_0;
    });

    for (const auto& triangle : volumeConstraints.triangles)
    {
        unsigned int v1 = triangle.v1;
        unsigned int v2 = triangle.v2;
        unsigned int v3 = triangle.v3;
        volumeConstraints.gradC.push_back([=](const std::vector<glm::vec3>& x) -> std::vector<glm::vec3> {
            glm::vec3 n1(0.0f), n2(0.0f), n3(0.0f);
            n1 += factor * glm::cross(x[v2], x[v3]);
            n2 += factor * glm::cross(x[v3], x[v1]);
            n3 += factor * glm::cross(x[v1], x[v2]);
            return { n1, n2, n3 };
        });
    }
}

void Mesh::setCandidateMeshes(const std::vector<Mesh*>& meshes)
{
    for (const auto* mesh : meshes)
    {
        if (mesh == this) continue; // Skip self
        m_candidateMeshes.push_back(mesh);
    }
}

void Mesh::constructEnvCollisionConstraints()
{
    // TODO
}

void Mesh::initVertices()
{
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

void Mesh::initVertexNormals()
{
    glGenVertexArrays(1, &m_vertexNormalVAO);
    glGenBuffers(1, &m_vertexNormalVBO);

    glBindVertexArray(m_vertexNormalVAO);
    glBindBuffer(GL_ARRAY_BUFFER, m_vertexNormalVBO);

    glBufferData(GL_ARRAY_BUFFER, m_vertexNormals.size() * 2 * sizeof(glm::vec3), nullptr, GL_DYNAMIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)0);

    glBindVertexArray(0);
}

// TODO : combine in 1 function with Mesh::initVertexNormals()
void Mesh::initFaceNormals()
{
    glGenVertexArrays(1, &m_faceNormalVAO);
    glGenBuffers(1, &m_faceNormalVBO);

    glBindVertexArray(m_faceNormalVAO);
    glBindBuffer(GL_ARRAY_BUFFER, m_faceNormalVBO);

    glBufferData(GL_ARRAY_BUFFER, m_faceNormals.size() * 2 * sizeof(glm::vec3), nullptr, GL_DYNAMIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)0);

    glBindVertexArray(0);
}

Mesh::Mesh(const std::string& name, const std::string& meshPath)
    : m_name(name),
      m_meshPath(meshPath),
      m_vertexNormalLength(0.5f),
      m_faceNormalLength(1.0f)
{
    loadObjData(meshPath);
    initVertices();
    initVertexNormals();
    initFaceNormals();
}

void Mesh::update()
{
    // update m_vertices positions
    size_t n = m_positions.size();
    for (size_t i = 0; i < n; ++i)
    {
        const glm::vec3& updatedPosition = m_positions[i];
        const auto& duplicates = m_duplicatePositionIndices[i];
        for (unsigned int idx : duplicates)
        {
            m_vertices[idx].position = updatedPosition;
        }
    }

    // update m_vertices normals
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

        // update vertex and face normals
        m_vertexNormals[tri] = faceNormal;

        if (tri < m_faceNormals.size())
        {
            m_faceNormals[tri] = faceNormal;
        }
    }
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

void Mesh::drawVertexNormals()
{
    std::vector<glm::vec3> lineVertices;
    for (const auto& v : m_vertices)
    {
        lineVertices.push_back(v.position);
        lineVertices.push_back(v.position + v.normal * m_vertexNormalLength);
    }

    glBindVertexArray(m_vertexNormalVAO);
    glBindBuffer(GL_ARRAY_BUFFER, m_vertexNormalVBO);
    glBufferSubData(GL_ARRAY_BUFFER, 0, lineVertices.size() * sizeof(glm::vec3), lineVertices.data());

    glDrawArrays(GL_LINES, 0, static_cast<GLsizei>(lineVertices.size()));

    glBindVertexArray(0);
}

void Mesh::drawFaceNormals()
{
    std::vector<glm::vec3> lineVertices;
    for (size_t i = 0; i + 2 < m_indices.size(); i += 3)
    {
        unsigned int idx0 = m_indices[i];
        unsigned int idx1 = m_indices[i + 1];
        unsigned int idx2 = m_indices[i + 2];

        glm::vec3 centroid = (m_vertices[idx0].position + m_vertices[idx1].position + m_vertices[idx2].position) / 3.0f;
        glm::vec3 normal = m_faceNormals[i / 3];

        lineVertices.push_back(centroid);
        lineVertices.push_back(centroid + normal * m_faceNormalLength);
    }

    glBindVertexArray(m_faceNormalVAO);
    glBindBuffer(GL_ARRAY_BUFFER, m_faceNormalVBO);
    glBufferSubData(GL_ARRAY_BUFFER, 0, lineVertices.size() * sizeof(glm::vec3), lineVertices.data());

    glDrawArrays(GL_LINES, 0, static_cast<GLsizei>(lineVertices.size()));

    glBindVertexArray(0);
}

void Mesh::destroy()
{
    glDeleteVertexArrays(1, &m_VAO);
    glDeleteBuffers(1, &m_VBO);
    glDeleteBuffers(1, &m_EBO);

    glDeleteVertexArrays(1, &m_vertexNormalVAO);
    glDeleteBuffers(1, &m_vertexNormalVBO);

    glDeleteVertexArrays(1, &m_faceNormalVAO);
    glDeleteBuffers(1, &m_faceNormalVBO);
}