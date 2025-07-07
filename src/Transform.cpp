#include "Transform.hpp"

#include <iostream>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/string_cast.hpp>

Transform::Transform()
    : m_mass(1.0f),
      m_position(glm::vec3(0.0f)),
      m_velocity(glm::vec3(0.0f)),
      m_acceleration(glm::vec3(0.0f)),
      m_projection(glm::mat4(1.0f)),
      m_view(glm::mat4(1.0f)),
      m_model(glm::mat4(1.0f))
{
}

void Transform::setProjection(const Camera& camera)
{
    m_projection = glm::perspective(
        glm::radians(camera.getFOV()),
        camera.getAspectRatio(),
        camera.getNearPlane(),
        camera.getFarPlane()
    );
}

void Transform::setModel(const glm::mat4& model)
{
    m_model = model;
}

void Transform::setView(const Camera& camera)
{
    m_view = glm::lookAt(
        camera.getPosition(),
        camera.getPosition() + camera.getFront(),
        camera.getUp()
    );
}
