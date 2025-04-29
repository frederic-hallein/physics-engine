#include "Camera.hpp"

void scrollCallback(GLFWwindow* window, double xoffset, double yoffset)
{
    Camera* camera = static_cast<Camera*>(glfwGetWindowUserPointer(window));
    if (!camera)
    {
        std::cerr << "Error: Camera instance is null in scrollCallback" << '\n';
        return;
    }

    float scrollSpeed = static_cast<float>(150.0 * camera->m_deltaTime);
    camera->setPosition(camera->getPosition() + camera->getFront() * static_cast<float>(yoffset) * scrollSpeed);

}

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
    glfwSetWindowUserPointer(m_window, this);
    glfwSetScrollCallback(m_window, scrollCallback);
}

void Camera::move()
{
    float cameraSpeed = static_cast<float>(30 * m_deltaTime);

    if (glfwGetKey(m_window, GLFW_KEY_A) == GLFW_PRESS)
        m_cameraPos -= glm::normalize(glm::cross(m_cameraFront, m_cameraUp)) * cameraSpeed;
    if (glfwGetKey(m_window, GLFW_KEY_D) == GLFW_PRESS)
        m_cameraPos += glm::normalize(glm::cross(m_cameraFront, m_cameraUp)) * cameraSpeed;
    if (glfwGetKey(m_window, GLFW_KEY_S) == GLFW_PRESS)
        m_cameraPos -= glm::normalize(m_cameraFront) * cameraSpeed;
    if (glfwGetKey(m_window, GLFW_KEY_W) == GLFW_PRESS)
        m_cameraPos += glm::normalize(m_cameraFront) * cameraSpeed;

    if (glfwGetKey(m_window, GLFW_KEY_SPACE) == GLFW_PRESS)
        m_cameraPos += m_cameraUp * cameraSpeed;
    if (glfwGetKey(m_window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
        m_cameraPos -= m_cameraUp * cameraSpeed;
}