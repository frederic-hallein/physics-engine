#pragma once

#include <string>
#include <glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <optional>

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
        float& k,
        Shader shader,
        Shader normalShader,
        Mesh mesh,
        std::optional<Texture> texture = std::nullopt,
        bool isStatic = true
    );
    ~Object();

    std::string getName() const { return m_name; }

    void update(float deltaTime);
    void render();

    void setPolygonMode(GLenum mode) { m_polygonMode = mode; }
    GLenum getPolygonMode() const { return m_polygonMode; }

    const bool isStatic() const { return m_isStatic; }

    Transform& getTransform() { return m_transform; }
    std::vector<Transform>& getVertexTransforms() { return m_vertexTransforms; }
    Mesh& getMesh() { return m_mesh; }
    const std::vector<float>& getMass() const { return m_M; }

    void resetVertexTransforms();

private:
    std::string m_name;
    Transform m_transform;
    Shader m_shader;
    Shader m_normalShader;
    Mesh m_mesh;
    std::optional<Texture> m_texture;
    bool m_isStatic;
    GLenum m_polygonMode;


    std::vector<Transform> m_initialVertexTransforms;
    std::vector<Transform> m_vertexTransforms;
    std::vector<float> m_M;

};