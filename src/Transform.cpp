#include "Transform.hpp"

#include <iostream>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/string_cast.hpp>

Transform::Transform()
    : m_mass(1.0f),
      m_position(glm::vec3(1.0f)),
      m_velocity(glm::vec3(0.0f)),
      m_projection(glm::mat4(1.0f)),
      m_view(glm::mat4(1.0f)),
      m_model(glm::mat4(1.0f))
{
}

void Transform::setProjection(float fov, float aspectRatio, float nearPlane, float farPlane)
{
    m_projection = glm::perspective(glm::radians(fov), aspectRatio, nearPlane, farPlane);
}

void Transform::setModel(const glm::mat4& model)
{
    m_position = glm::vec3(model[3][0], model[3][1], model[3][2]);
    m_model = model;
}

void Transform::setView(const glm::vec3& position, const glm::vec3& front, const glm::vec3& up)
{
    m_view = glm::lookAt(position, position + front, up);
}
