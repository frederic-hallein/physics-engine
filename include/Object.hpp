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
    virtual void update(float deltaTime) = 0;
    virtual void render() = 0;
    // virtual void setTransform(const Transform& transform) = 0;
    virtual Transform& getTransform() = 0;
    virtual std::vector<Transform>& getVertexTransforms() = 0;
    virtual const bool isStatic() const = 0;
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
    void update(float deltaTime) override;
    void render() override;

    // void setTransform(const Transform& transform) override;
    Transform& getTransform() override { return m_transform; }
    std::vector<Transform>& getVertexTransforms() override { return m_vertexTransforms; }

    const bool isStatic() const override { return m_isStatic; }
protected:
    float m_totalMass;
    Transform m_transform;
    std::vector<Transform> m_vertexTransforms;
    Shader m_shader;
    Mesh m_mesh;
    bool m_isStatic;
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