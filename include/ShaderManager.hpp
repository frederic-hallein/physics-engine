#pragma once

#include <unordered_map>
#include <memory>
#include <string>

#include "Shader.hpp"

class ShaderManager
{
public:

    void addShader(std::unique_ptr<Shader> shader);
    Shader& getShader(const std::string& name) { return *shaders[name]; }
    void deleteAllShaders();

private:
    std::unordered_map<std::string, std::unique_ptr<Shader>> shaders;
};