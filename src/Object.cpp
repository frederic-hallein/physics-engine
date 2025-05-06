#include "Object.hpp"

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/string_cast.hpp>

Cube::Cube(
    Transform transform,
    Shader& shader,
    Mesh& mesh,
    bool isStatic
)
    : m_totalMass(0.0f),
      m_transform(std::move(transform)),
      m_shader(&shader),
      m_mesh(&mesh),
      m_isStatic(isStatic)
{
    // initialize vertex transforms
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

    // calculate total mass used for COM
    for (const auto& vertexTransform : m_vertexTransforms)
        m_totalMass += vertexTransform.getMass();

    std::cout << "Cube created" << '\n';
}

void Cube::update()
{
    // Calculate the center of mass of the vertex transforms
    glm::vec3 centerOfMass(0.0f);
    for (const auto& vertexTransform : m_vertexTransforms)
    {
        centerOfMass += vertexTransform.getMass() * vertexTransform.getPosition();
    }

    centerOfMass /= m_totalMass;

    // Extract the current rotation from the model matrix
    glm::mat4 currentModelMatrix = m_transform.getModelMatrix();
    glm::mat3 rotationMatrix = glm::mat3(currentModelMatrix); // Extract rotation (upper-left 3x3)

    // Create a new translation matrix for the updated position
    glm::mat4 translationMatrix = glm::translate(glm::mat4(1.0f), centerOfMass);

    // Combine the rotation and translation matrices
    glm::mat4 updatedModelMatrix = translationMatrix * glm::mat4(rotationMatrix);

    // Update the model matrix with the new position and preserved rotation
    m_transform.setModel(updatedModelMatrix);
}

void Cube::render()
{
    m_shader->useProgram();

    // Set projection matrix (same for all vertex transforms)
    int projectionLoc = glGetUniformLocation(m_shader->getID(), "projection");
    glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(m_transform.getProjectionMatrix()));

    // Set model matrix for the current vertex transform
    int modelLoc = glGetUniformLocation(m_shader->getID(), "model");
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(m_transform.getModelMatrix()));

    // Set view matrix for the current vertex transform
    int viewLoc = glGetUniformLocation(m_shader->getID(), "view");
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(m_transform.getViewMatrix()));

    // Draw the mesh for the current vertex transform
    m_mesh->draw();
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

