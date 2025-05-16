#pragma once

#include <iostream>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <imgui.h>

class Camera
{
public:
    Camera(
        glm::vec3 cameraPos,
        glm::vec3 cameraFront,
        glm::vec3 cameraUp,
        float FOV,
        float aspectRatio,
        float nearPlane,
        float farPlane,
        GLFWwindow* window
    );

    void move();

    const glm::vec3& getPosition() const { return m_cameraPos; }
    const glm::vec3& getFront()    const { return m_cameraFront; }
    const glm::vec3& getUp()       const { return m_cameraUp; }
    float getFOV()                 const { return m_FOV; }
    float getAspectRatio()         const { return m_aspectRatio; }
    float getNearPlane()           const { return m_nearPlane; }
    float getFarPlane()            const { return m_farPlane; }

    void setPosition(const glm::vec3& position) { m_cameraPos = position; } // Setter for m_cameraPos
    void setDeltaTime(float deltaTime) { m_deltaTime = deltaTime; }
private:
    glm::vec3 m_cameraPos;
    glm::vec3 m_cameraFront;
    glm::vec3 m_cameraUp;

    float m_FOV;
    float m_aspectRatio;
    float m_nearPlane;
    float m_farPlane;

    float m_deltaTime = 0.0f;
    GLFWwindow* m_window;

    friend void scrollCallback(GLFWwindow* window, double xoffset, double yoffset);

};