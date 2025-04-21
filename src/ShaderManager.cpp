#include "ShaderManager.hpp"

void ShaderManager::addShader(const std::string& name, const std::string& vertexPath, const std::string& fragmentPath)
{
    shaders[name] = std::make_unique<Shader>(vertexPath.c_str(), fragmentPath.c_str());
}

void ShaderManager::deleteAllShaders()
{
    for (auto& [name, shader] : shaders)
    {
        shader->deleteProgram();
    }
}