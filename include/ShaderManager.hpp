#pragma once

#include <unordered_map>
#include <memory>
#include <string>
#include "Shader.hpp"

class ShaderManager
{
public:

    void addShader(const std::string& name, const std::string& vertexPath, const std::string& fragmentPath);
    Shader* getShader(const std::string& name) { return shaders[name].get(); }
    void useShader(const std::string& name) { shaders[name]->useProgram(); };
    void deleteAllShaders();

private:
    std::unordered_map<std::string, std::unique_ptr<Shader>> shaders;
};