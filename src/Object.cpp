#include "Object.hpp"

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/string_cast.hpp>

Cube::Cube(
    Transform transform,
    Shader shader,
    Mesh mesh,
    bool isStatic
)
    : m_totalMass(0.0f),
      m_transform(std::move(transform)),
      m_shader(std::move(shader)),
      m_mesh(mesh),
      m_isStatic(isStatic)
{

}


void Cube::update(float deltaTime)
{
    m_mesh.update();
}

void Cube::render()
{
    m_shader.useProgram();
    // Set projection matrix (same for all vertex transforms)
    int projectionLoc = glGetUniformLocation(m_shader.getID(), "projection");
    glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(m_transform.getProjectionMatrix()));

    // Set model matrix for the current vertex transform
    int modelLoc = glGetUniformLocation(m_shader.getID(), "model");
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(m_transform.getModelMatrix()));

    // Set view matrix for the current vertex transform
    int viewLoc = glGetUniformLocation(m_shader.getID(), "view");
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(m_transform.getViewMatrix()));

    m_mesh.draw();
}

DirtBlock::DirtBlock(
    Transform transform,
    Shader shader,
    Mesh mesh,
    Texture texture,
    bool isStatic
)
    : Cube(transform, shader, mesh, isStatic),
      m_texture(std::move(texture))
{
    std::cout << "DirtBlock created" << '\n';
}

void DirtBlock::render()
{
    m_texture.bind();
    Cube::render();
}

