#pragma once

#include <string>
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "ShaderManager.hpp"
#include "MeshManager.hpp"
#include "TextureManager.hpp"
#include "Camera.hpp"
#include "Object.hpp"

class Scene
{
public:
    Scene(
        const std::string& name,
        std::unique_ptr<ShaderManager> shaderManager,
        std::unique_ptr<MeshManager> meshManager,
        std::unique_ptr<TextureManager> textureManager,
        std::unique_ptr<Camera>
    );

    void update(float deltaTime);
    void render(float deltaTime);
    void clear();

private:
    void applyGravity(Transform& transform, float deltaTime);

    std::string m_name;

    std::unique_ptr<ShaderManager> m_shaderManager;
    std::unique_ptr<MeshManager> m_meshManager;
    std::unique_ptr<TextureManager> m_textureManager;

    std::unique_ptr<Camera> m_camera;

    std::vector<std::unique_ptr<Object>> m_objects;
};