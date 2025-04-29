#pragma once

#include <string>
#include <glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Transform.hpp"
#include "Shader.hpp"
#include "Mesh.hpp"
#include "Texture.hpp"

class Object
{
public:
    Object() = default;
    virtual ~Object() = default;
    virtual void render() = 0;
    virtual Transform& getTransform() = 0;
};

class Cube : public Object
{
public:
    Cube() = default;
    Cube(
        Transform transform,
        Shader& shader,
        Mesh& mesh
    );
    void render() override;
    void addBody(); // rigid or soft

    Transform& getTransform() override { return m_transform; }

protected:
    Transform m_transform;
    Shader* m_shader;
    Mesh* m_mesh;
};

class DirtBlock: public Cube
{
public:
    DirtBlock() = default;
    DirtBlock(
        Transform transform,
        Shader& shader,
        Mesh& mesh,
        Texture& texture
    );
    void render() override;
private:
    Texture* m_texture;
};