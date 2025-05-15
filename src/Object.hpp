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
    Object(
        Transform transform,
        Shader shader,
        Mesh mesh,
        bool isStatic = true
    );
    virtual ~Object() = default;

    void update(float deltaTime);
    virtual void render();

    const bool isStatic() const { return m_isStatic; }

    Transform& getTransform() { return m_transform; }
    std::vector<Transform>& getVertexTransforms() { return m_vertexTransforms; }

protected:
    Transform m_transform;
    Shader m_shader;
    Mesh m_mesh;
    bool m_isStatic;

    float m_totalMass;
    std::vector<Transform> m_vertexTransforms;
};

class Cube : public Object
{
public:
    Cube() = default;
    Cube(
        Transform transform,
        Shader shader,
        Mesh mesh,
        bool isStatic = true
    );
};

class DirtBlock: public Cube
{
public:
    DirtBlock() = default;
    DirtBlock(
        Transform transform,
        Shader shader,
        Mesh mesh,
        Texture texture,
        bool isStatic = true
    );
    void render() override;
private:
    Texture m_texture;
};

class Sphere : public Object
{
public:
    Sphere() = default;
    Sphere(
        Transform transform,
        Shader shader,
        Mesh mesh,
        bool isStatic = true
    );

};