#pragma once

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

class Shader
{
public:
    unsigned int ID;

    Shader() = default;
    Shader(const char* vertexPath, const char* fragmentPath);

    void useProgram();
    void deleteProgram();
    void setBool(const std::string& name, bool value) const;
    void setInt(const std::string& name, int value) const;
    void setFloat(const std::string& name, float value) const;
};