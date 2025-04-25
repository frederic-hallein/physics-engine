#include "ShaderManager.hpp"

void ShaderManager::addShader(std::unique_ptr<Shader> shader)
{
    shaders[shader->getName()] = std::move(shader);
}

void ShaderManager::deleteAllShaders()
{
    for (auto& [name, shader] : shaders)
    {
        shader->deleteProgram();
    }
}