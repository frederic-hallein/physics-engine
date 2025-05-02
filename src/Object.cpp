#include "Object.hpp"

Cube::Cube(
    Transform transform,
    Shader& shader,
    Mesh& mesh,
    bool isStatic
)
    : m_transform(std::move(transform)),
      m_shader(&shader),
      m_mesh(&mesh),
      m_isStatic(isStatic)
{
    for (const auto& pos : m_mesh->getPositions())
    {
        Transform vertexTransform;
        if (!m_isStatic)
            vertexTransform.makeNotStatic();

        // Set the model matrix relative to the object's transform
        glm::mat4 relativeTranslation = glm::translate(glm::mat4(1.0f), pos);
        glm::mat4 modelMatrix = m_transform.getModelMatrix() * relativeTranslation;

        vertexTransform.setModel(modelMatrix);
        m_vertexTransforms.push_back(vertexTransform);
    }


    std::cout << "Cube created" << '\n';
}

void Cube::render()
{
    m_shader->useProgram();

    // Set projection matrix (same for all vertex transforms)
    int projectionLoc = glGetUniformLocation(m_shader->getID(), "projection");
    glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(m_transform.getProjectionMatrix()));

    // Iterate through all vertex transforms
    for (const auto& vertexTransform : m_vertexTransforms)
    {
        // Set model matrix for the current vertex transform
        int modelLoc = glGetUniformLocation(m_shader->getID(), "model");
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(vertexTransform.getModelMatrix()));

        // Set view matrix for the current vertex transform
        int viewLoc = glGetUniformLocation(m_shader->getID(), "view");
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(vertexTransform.getViewMatrix()));

    }
    // Draw the mesh for the current vertex transform
    m_mesh->draw();

}

void Cube::addBody()
{
    // add rigid or soft
}

DirtBlock::DirtBlock(
    Transform transform,
    Shader& shader,
    Mesh& mesh,
    Texture& texture,
    bool isStatic
)
    : Cube(transform, shader, mesh, isStatic),
      m_texture(&texture)
{
    std::cout << "DirtBlock created" << '\n';
}

void DirtBlock::render()
{
    m_texture->bind();
    Cube::render();
}

