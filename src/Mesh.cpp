#include "Mesh.hpp"

// #define GLM_ENABLE_EXPERIMENTAL
// #include <glm/gtx/string_cast.hpp>

void Mesh::constructDistanceConstraintVertices(const aiMesh* mesh)
{
    struct IndexEdge {
        int v1, v2;
        bool operator==(const IndexEdge& other) const {
            return (v1 == other.v1 && v2 == other.v2) || (v1 == other.v2 && v2 == other.v1);
        }
        bool operator<(const IndexEdge& other) const {
            int a1 = std::min(v1, v2), a2 = std::max(v1, v2);
            int b1 = std::min(other.v1, other.v2), b2 = std::max(other.v1, other.v2);
            return std::tie(a1, a2) < std::tie(b1, b2);
        }
    };

    std::set<IndexEdge> uniqueEdges;

    // For each triangle, add its three edges (by unique position indices)
    for (unsigned int i = 0; i < mesh->mNumFaces; ++i) {
        const aiFace& face = mesh->mFaces[i];
        if (face.mNumIndices != 3) continue;

        // Map mesh vertex indices to unique position indices
        int idx[3];
        for (int j = 0; j < 3; ++j) {
            glm::vec3 pos(mesh->mVertices[face.mIndices[j]].x,
                          mesh->mVertices[face.mIndices[j]].y,
                          mesh->mVertices[face.mIndices[j]].z);
            auto it = std::find(positions.begin(), positions.end(), pos);
            idx[j] = static_cast<int>(std::distance(positions.begin(), it));
        }

        uniqueEdges.insert(IndexEdge{idx[0], idx[1]});
        uniqueEdges.insert(IndexEdge{idx[1], idx[2]});
        uniqueEdges.insert(IndexEdge{idx[2], idx[0]});
    }

    distanceConstraintVertices.clear();
    for (const auto& e : uniqueEdges) {
        Edge edge;
        edge.v1 = e.v1;
        edge.v2 = e.v2;
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
    size_t n = positions.size();
    for (size_t i = 0; i < n; ++i) {
        const glm::vec3& updatedPosition = positions[i];
        const auto& duplicates = m_duplicatePositionIndices[i];
        for (unsigned int idx : duplicates) {
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