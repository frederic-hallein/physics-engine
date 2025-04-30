#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Camera.hpp"

class Transform
{
public:
    Transform();

    void setPosition(const glm::vec3& position) { m_position = position; }
    void setVelocity(const glm::vec3& velocity) { m_velocity = velocity; }
    void setProjection(float fov, float aspectRatio, float nearPlane, float farPlane);
    void setModel(const glm::mat4& model);
    void setView(const glm::vec3& position, const glm::vec3& front, const glm::vec3& up);

    const glm::vec3& getPosition()         const { return m_position; }
    const glm::vec3& getVelocity()         const { return m_velocity; }
    const glm::mat4& getProjectionMatrix() const { return m_projection; }
    const glm::mat4& getModelMatrix()      const { return m_model; };
    const glm::mat4& getViewMatrix()       const { return m_view; };

    void makeNonStatic() { m_static = false; }
    const bool isStatic() const { return m_static; }

private:
    glm::vec3 m_position;
    glm::vec3 m_velocity;
    glm::vec3 m_front;
    glm::vec3 m_up;

    glm::mat4 m_projection;
    glm::mat4 m_model;
    glm::mat4 m_view;

    bool m_static = true;
};