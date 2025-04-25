#include "Scene.hpp"


Scene::Scene(
    const std::string& name,
    std::unique_ptr<ShaderManager> shaderManager,
    std::unique_ptr<MeshManager> meshManager,
    std::unique_ptr<TextureManager> textureManager
)
    : m_name(name),
      m_shaderManager(std::move(shaderManager)),
      m_meshManager(std::move(meshManager)),
      m_textureManager(std::move(textureManager))
{
    // create dirtblock
    Shader& shader = m_shaderManager->getShader("dirtBlockShader");
    Mesh& mesh = m_meshManager->getMesh("cubeMesh");
    Texture& texture = m_textureManager->getTexture("dirtBlockTexture");

    auto dirtBlock = std::make_unique<DirtBlock>(
        shader,
        mesh,
        texture
    );

    m_objects.push_back(std::move(dirtBlock));

}

void Scene::render()
{
    glClearColor(0.2f, 0.2f, 0.8f, 1.0f); // background
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    for (const auto& object : m_objects)
    {
        object->render();
    }
}

void Scene::clear()
{
    m_textureManager->deleteAllTextures();
    m_meshManager->deleteAllMeshes();
    m_shaderManager->deleteAllShaders();
    m_objects.clear();
}