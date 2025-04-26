#include "Transform.hpp"

#include <iostream>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/string_cast.hpp>

Transform::Transform()
    : m_projection(glm::mat4(1.0f)),
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
    m_model = model;
}

void Transform::setView(const glm::vec3& position, const glm::vec3& front, const glm::vec3& up)
{
    m_position = position;
    m_front = front;
    m_up = up;
    m_view = glm::lookAt(m_position, m_position + m_front, m_up);
}