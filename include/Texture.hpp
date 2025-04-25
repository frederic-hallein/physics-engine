#pragma once

#include <string>
#include <glad.h>
#include <stb_image.h>
#include <iostream>

class Texture
{
public:
    Texture() = default;
    Texture(const std::string& name, const std::string& filePath);

    const std::string getName()        const { return m_name; }
    const std::string getTexturePath() const { return m_texturePath; }
    const unsigned int getID()         const { return m_ID; }

    void bind();
    void deleteTexture();

private:
    std::string m_name;
    std::string m_texturePath;

    unsigned int m_ID;
};