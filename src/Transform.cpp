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

void Transform::setProjection(const Camera& camera)
{
    float fov = camera.getFOV();
    float aspectRatio = camera.getAspectRatio();
    float nearPlane = camera.getNearPlane();
    float farPlane = camera.getFarPlane();
    m_projection = glm::perspective(glm::radians(fov), aspectRatio, nearPlane, farPlane);
}

void Transform::setModel(const glm::mat4& model)
{
    m_position = glm::vec3(model[3][0], model[3][1], model[3][2]);
    m_model = model;
}

void Transform::setView(const Camera& camera)
{
    glm::vec3 position = camera.getPosition();
    glm::vec3 front = camera.getFront();
    glm::vec3 up = camera.getUp();
    m_view = glm::lookAt(position, position + front, up);
}
