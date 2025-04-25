#include "Object.hpp"

const int unsigned SCREEN_WIDTH = 1080;
const int unsigned SCREEN_HEIGHT = 720;

// camera
glm::vec3 cameraPos   = glm::vec3(0.0f, 0.0f,  3.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp    = glm::vec3(0.0f, 1.0f,  0.0f);

float fov   =  45.0f;

// void Object::render()
// {
//     m_shader->useProgram();
//     m_mesh->draw();
//     m_texture->bind();

//     // // projection transformation
//     // glm::mat4 projection = glm::perspective(glm::radians(fov), (float)m_screenWidth / (float)m_screenHeight, 0.1f, 100.0f);
//     // int projectionLoc = glGetUniformLocation(shaderManager->getShader("basic")->ID, "projection");
//     // glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));

//     // // model transformation
//     // glm::mat4 model = glm::mat4(1.0f);
//     // model = glm::rotate(model, (float)glfwGetTime() * glm::radians(180.0f), glm::vec3(2.0f, 1.0f, 0.5f));
//     // int modelLoc = glGetUniformLocation(shaderManager->getShader("basic")->ID, "model");
//     // glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

//     // // camera/view transformation
//     // glm::mat4 view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
//     // int viewLoc = glGetUniformLocation(shaderManager->getShader("basic")->ID, "view");
//     // glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
// }

Cube::Cube(
    Shader& shader,
    Mesh& mesh
)
    : m_shader(&shader),
      m_mesh(&mesh)
{

}

void Cube::render()
{
    m_shader->useProgram();
    m_mesh->draw();

    // projection transformation
    glm::mat4 projection = glm::perspective(glm::radians(fov), (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, 0.1f, 100.0f);
    int projectionLoc = glGetUniformLocation(m_shader->getID(), "projection");
    glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));

    // model transformation
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::rotate(model, (float)glfwGetTime() * glm::radians(180.0f), glm::vec3(2.0f, 1.0f, 0.5f));
    int modelLoc = glGetUniformLocation(m_shader->getID(), "model");
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

    // camera/view transformation
    glm::mat4 view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
    int viewLoc = glGetUniformLocation(m_shader->getID(), "view");
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
}

void Cube::addBody()
{

}

DirtBlock::DirtBlock(
    Shader& shader,
    Mesh& mesh,
    Texture& texture
)
    : Cube(shader, mesh),
      m_texture(&texture)
{

}

void DirtBlock::render()
{
    Cube::render();
    m_texture->bind();
}

