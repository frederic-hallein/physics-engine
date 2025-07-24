#include "ShaderManager.hpp"

void ShaderManager::addShaders(std::vector<std::unique_ptr<Shader>> shaders)
{
    for (auto& shader : shaders)
    {
        m_shaders[shader->getName()] = std::move(shader);
    }
}

Shader& ShaderManager::getShader(const std::string& name)
{
    auto it = m_shaders.find(name);
    if (it == m_shaders.end())
    {
        throw std::runtime_error("ShaderManager: Shader '" + name + "' not found!");
    }
    return *(it->second);
}

void ShaderManager::deleteAllShaders()
{
    for (auto& [name, shader] : m_shaders)
    {
        shader->deleteProgram();
    }
}