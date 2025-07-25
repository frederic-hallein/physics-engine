#include <string>
#include <glad.h>
#include <GLFW/glfw3.h>

#include "Shader.hpp"

static void checkCompileErrors(unsigned int shader, std::string type)
{
    int success;
    char infoLog[1024];
    if (type != "PROGRAM")
    {
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            glGetShaderInfoLog(shader, 1024, NULL, infoLog);
            std::cout << "Shader: Shader Compilation Error of type: " << type << "\n" << infoLog << "\n";
        }
    }
    else
    {
        glGetProgramiv(shader, GL_LINK_STATUS, &success);
        if (!success)
        {
            glGetProgramInfoLog(shader, 1024, NULL, infoLog);
            std::cout << "Shader: Program Linking Error of type: " << type << "\n" << infoLog << "\n";
        }
    }
}

static void getVertexAndFragmentSourceCode(const std::string& vertexPath, const std::string& fragmentPath,
                                           std::string& vShaderCode, std::string& fShaderCode)
{
    std::ifstream vShaderFile;
    std::ifstream fShaderFile;
    std::stringstream vShaderStream, fShaderStream;

    // Ensure ifstream objects can throw exceptions
    vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

    try
    {
        // Open files
        vShaderFile.open(vertexPath);
        fShaderFile.open(fragmentPath);

        // Read file contents into streams
        vShaderStream << vShaderFile.rdbuf();
        fShaderStream << fShaderFile.rdbuf();

        // Close file handlers
        vShaderFile.close();
        fShaderFile.close();

        // Convert stream into string
        vShaderCode = vShaderStream.str();
        fShaderCode = fShaderStream.str();
    }
    catch (std::ifstream::failure& e)
    {
        std::cerr << "ERROR::SHADER::FILE_NOT_SUCCESSFULLY_READ: " << e.what() << std::endl;
    }
}

void compileShaders(unsigned int& ID, const std::string& vShaderCode, const std::string& fShaderCode)
{
    const char* vShaderCodeCStr = vShaderCode.c_str();
    const char* fShaderCodeCStr = fShaderCode.c_str();

    unsigned int vertex, fragment;
    // vertex shader
    vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, &vShaderCodeCStr, NULL);
    glCompileShader(vertex);
    checkCompileErrors(vertex, "VERTEX");
    // fragment Shader
    fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, &fShaderCodeCStr, NULL);
    glCompileShader(fragment);
    checkCompileErrors(fragment, "FRAGMENT");

    // shader Program
    ID = glCreateProgram();
    glAttachShader(ID, vertex);
    glAttachShader(ID, fragment);
    glLinkProgram(ID);
    checkCompileErrors(ID, "PROGRAM");

    // delete the shaders as they're linked into our program now and no longer necessary
    glDeleteShader(vertex);
    glDeleteShader(fragment);
}

Shader::Shader(const std::string& name, const std::string& vertexPath, const std::string& fragmentPath)
    : m_name(name), m_vertexPath(vertexPath), m_fragmentPath(fragmentPath)
{
    std::string vShaderCode;
    std::string fShaderCode;
    getVertexAndFragmentSourceCode(vertexPath, fragmentPath, vShaderCode, fShaderCode);
    compileShaders(m_ID, vShaderCode, fShaderCode);
}

void Shader::useProgram()
{
    glUseProgram(m_ID);
}

void Shader::destroy()
{
    glDeleteProgram(m_ID);
}

void Shader::setUniform(const std::string& name, const glm::vec3& color)
{
    int vertexColorLocation = glGetUniformLocation(m_ID, name.c_str());
    glUniform4f(vertexColorLocation, color[0], color[1], color[2], 1.0f);
}

// void Shader::setBool(const std::string& name, bool value) const
// {
//     glUniform1i(glGetUniformLocation(m_ID, name.c_str()), (int)value);
// }

// void Shader::setInt(const std::string& name, int value) const
// {
//     glUniform1i(glGetUniformLocation(m_ID, name.c_str()), value);
// }

// void Shader::setFloat(const std::string& name, float value) const
// {
//     glUniform1f(glGetUniformLocation(m_ID, name.c_str()), value);
// }

