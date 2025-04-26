#include "Camera.hpp"

// // timing
// float deltaTime = 0.0f;	// time between current frame and last frame
// float lastFrame = 0.0f;

// void scrollCallback(GLFWwindow* window, double xoffset, double yoffset)
// {
//     float scrollSpeed = static_cast<float>(150.0 * deltaTime);
//     m_cameraPos += m_cameraFront * static_cast<float>(yoffset) * scrollSpeed;
// }

Camera::Camera(
    glm::vec3 cameraPos,
    glm::vec3 cameraFront,
    glm::vec3 cameraUp,
    float fov,
    float aspectRatio,
    float nearPlane,
    float farPlane,
    GLFWwindow* window
)
    : m_cameraPos(cameraPos),
      m_cameraFront(cameraFront),
      m_cameraUp(cameraUp),
      m_FOV(fov),
      m_aspectRatio(aspectRatio),
      m_nearPlane(nearPlane),
      m_farPlane(farPlane),
      m_window(window)
{
    // glfwSetScrollCallback(m_window, scrollCallback);
}