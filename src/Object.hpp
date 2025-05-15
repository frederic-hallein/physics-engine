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
        std::string name,
        Transform transform,
        Shader shader,
        Mesh mesh,
        bool isStatic = true
    );
    virtual ~Object();

    virtual std::string getName() const { return m_name; }

    void update(float deltaTime);
    virtual void render();

    void setPolygonMode(GLenum mode) { m_polygonMode = mode; }
    GLenum getPolygonMode() const { return m_polygonMode; }

    const bool isStatic() const { return m_isStatic; }

    Transform& getTransform() { return m_transform; }
    std::vector<Transform>& getVertexTransforms() { return m_vertexTransforms; }

protected:
    std::string m_name;
    Transform m_transform;
    Shader m_shader;
    Mesh m_mesh;
    bool m_isStatic;

    GLenum m_polygonMode;

    float m_totalMass;
    std::vector<Transform> m_vertexTransforms;
};

class Cube : public Object
{
public:
    Cube() = default;
    Cube(
        std::string name,
        Transform transform,
        Shader shader,
        Mesh mesh,
        bool isStatic = true
    );

    // std::string getName() const override { return m_name; }
};

class DirtBlock: public Cube
{
public:
    DirtBlock() = default;
    DirtBlock(
        std::string name,
        Transform transform,
        Shader shader,
        Mesh mesh,
        Texture texture,
        bool isStatic = true
    );
    // std::string getName() const override { return m_name; }

    void render() override;
private:
    Texture m_texture;
};

class Sphere : public Object
{
public:
    Sphere() = default;
    Sphere(
        std::string name,
        Transform transform,
        Shader shader,
        Mesh mesh,
        bool isStatic = true
    );

    // std::string getName() const override { return m_name; }

};