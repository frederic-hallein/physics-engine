#include "Mesh.hpp"

// #define GLM_ENABLE_EXPERIMENTAL
// #include <glm/gtx/string_cast.hpp>

void Mesh::constructDistanceConstraintVertices(const aiMesh* mesh) {
    struct TmpEdge
    {
        glm::vec3 v1;
        glm::vec3 v2;
    };

    std::vector<TmpEdge> allEdges;
    for (unsigned int i = 0; i < mesh->mNumFaces; ++i) {
        const aiFace& face = mesh->mFaces[i];
        if (face.mNumIndices != 3) continue; // Only triangles

        unsigned int idx0 = face.mIndices[0];
        unsigned int idx1 = face.mIndices[1];
        unsigned int idx2 = face.mIndices[2];

        glm::vec3 v1(mesh->mVertices[idx0].x, mesh->mVertices[idx0].y, mesh->mVertices[idx0].z);
        glm::vec3 v2(mesh->mVertices[idx1].x, mesh->mVertices[idx1].y, mesh->mVertices[idx1].z);
        glm::vec3 v3(mesh->mVertices[idx2].x, mesh->mVertices[idx2].y, mesh->mVertices[idx2].z);

        allEdges.push_back(TmpEdge{v1, v2});
        allEdges.push_back(TmpEdge{v2, v3});
        allEdges.push_back(TmpEdge{v3, v1});
    }

    std::vector<TmpEdge> uniqueEdges;
    auto edgeEquals = [](const TmpEdge& a, const TmpEdge& b) {
        return ((a.v1 == b.v1 && a.v2 == b.v2) || (a.v1 == b.v2 && a.v2 == b.v1));
    };

    for (const auto& edge : allEdges) {
        bool found = false;
        for (const auto& uEdge : uniqueEdges) {
            if (edgeEquals(edge, uEdge)) {
                found = true;
                break;
            }
        }
        if (!found) {
            uniqueEdges.push_back(edge);
        }
    }

    for (const auto& e : uniqueEdges) {
        int idx1 = -1, idx2 = -1;
        // Find index of v1 in positions
        for (size_t i = 0; i < positions.size(); ++i) {
            if (positions[i] == e.v1) {
                idx1 = static_cast<int>(i);
                break;
            }
        }
        // Find index of v2 in positions
        for (size_t i = 0; i < positions.size(); ++i) {
            if (positions[i] == e.v2) {
                idx2 = static_cast<int>(i);
                break;
            }
        }
        Edge edge;
        edge.v1 = idx1;
        edge.v2 = idx2;
        distanceConstraintVertices.push_back(edge);
    }
}

void Mesh::loadObjData(const std::string& filePath)
{
    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(
        filePath,
        aiProcess_Triangulate  | aiProcess_FlipUVs | aiProcess_GenSmoothNormals
    );

    if (!scene || !scene->HasMeshes()) {
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

        // keep track of only unique vertex positions
        auto it = std::find(positions.begin(), positions.end(), vertex.position);
        if (it == positions.end())
        {
            positions.push_back(vertex.position);
            m_duplicatePositionIndices.push_back({i});
        }
        else
        {
            size_t idx = std::distance(positions.begin(), it);
            m_duplicatePositionIndices[idx].push_back(i);
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

    constructDistanceConstraintVertices(mesh);

}

void Mesh::constructDistanceConstraints()
{
    for (const auto& edge : distanceConstraintVertices)
    {
        int v1 = edge.v1;
        int v2 = edge.v2;
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
        gradDistanceConstraints.push_back([=](const std::vector<glm::vec3>& x) -> std::vector<glm::vec3> {
            glm::vec3 n = (x[edge.v1] - x[edge.v2]) / (glm::distance(x[edge.v1], x[edge.v2]));
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
    glBufferData(GL_ARRAY_BUFFER, m_vertices.size() * sizeof(Vertex), &m_vertices[0], GL_STATIC_DRAW);

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
    for (size_t i = 0; i < positions.size(); ++i) {
        const glm::vec3& updatedPosition = positions[i];
        for (unsigned int idx : m_duplicatePositionIndices[i])
        {
            m_vertices[idx].position = updatedPosition;
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

void Mesh::deleteMesh()
{
    glDeleteVertexArrays(1, &m_VAO);
    glDeleteBuffers(1, &m_VBO);
    glDeleteBuffers(1, &m_EBO);
}