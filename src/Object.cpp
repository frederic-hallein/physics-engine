#include "Object.hpp"

Cube::Cube(
    Transform transform,
    Shader& shader,
    Mesh& mesh
)
    : m_transform(std::move(transform)),
      m_shader(&shader),
      m_mesh(&mesh)
{
    // m_shader->useProgram();
}

Cube::Cube(const Cube& other)
    : m_transform(other.m_transform),
      m_shader(other.m_shader),
      m_mesh(other.m_mesh)
{

}

Cube& Cube::operator=(const Cube& other)
{
    if (this != &other)
    {
        m_transform = other.m_transform;
        m_shader = other.m_shader;
        m_mesh = other.m_mesh;
    }
    return *this;
}

void Cube::render()
{
    m_shader->useProgram();

    // Set projection matrix
    int projectionLoc = glGetUniformLocation(m_shader->getID(), "projection");
    glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(m_transform.getProjectionMatrix()));

    // Set model matrix
    int modelLoc = glGetUniformLocation(m_shader->getID(), "model");
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(m_transform.getModelMatrix()));

    // Set view matrix
    int viewLoc = glGetUniformLocation(m_shader->getID(), "view");
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(m_transform.getViewMatrix()));

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
    Texture& texture
)
    : Cube(transform, shader, mesh),
      m_texture(&texture)
{
    std::cout << "DirtBlock created" << '\n';
}

void DirtBlock::render()
{
    m_texture->bind();
    Cube::render();
}

