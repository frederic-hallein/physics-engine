#pragma once

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <glm/glm.hpp>

class Shader
{
public:
    Shader() = default;
    Shader(const std::string& name, const std::string& vertexPath, const std::string& fragmentPath);

    const std::string getName()         const { return m_name; }
    const std::string getVertexPath()   const { return m_vertexPath; }
    const std::string getFragmentPath() const { return m_fragmentPath; }
    const unsigned int getID()          const { return m_ID; }

    void useProgram();
    void destroy();
    // void setBool(const std::string& name, bool value) const;
    // void setInt(const std::string& name, int value) const;
    // void setFloat(const std::string& name, float value) const;

    void setUniform(const std::string& name, const glm::vec3& color);

private:
    std::string m_name;
    std::string m_vertexPath;
    std::string m_fragmentPath;

    unsigned int m_ID;
};