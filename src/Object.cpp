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
    std::vector<glm::vec3>& positions = m_mesh.positions;
    glm::mat3 rot = glm::mat3(m_transform.getModelMatrix());
    glm::vec3 trans = glm::vec3(m_transform.getModelMatrix()[3]);

    for (auto& pos : positions)
    {
        glm::vec3 newPos = rot * pos + trans;
        pos = newPos;
        std::cout << glm::to_string(pos) << '\n';
    }

}

void Cube::update(float deltaTime)
{
    m_mesh.update();
}

void Cube::render()
{
    m_shader.useProgram();

    int projectionLoc = glGetUniformLocation(m_shader.getID(), "projection");
    glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(m_transform.getProjectionMatrix()));

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

