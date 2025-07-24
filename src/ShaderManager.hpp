#pragma once

#include <unordered_map>
#include <memory>
#include <string>

#include "Shader.hpp"

class ShaderManager
{
public:
    void addShaders(std::vector<std::unique_ptr<Shader>> shaders);
    Shader& getShader(const std::string& name);
    void deleteAllShaders();

private:
    std::unordered_map<std::string, std::unique_ptr<Shader>> m_shaders;
};