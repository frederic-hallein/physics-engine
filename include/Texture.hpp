#pragma once

#include <string>
#include <glad.h>
#include <stb_image.h>
#include <iostream>

class Texture
{
public:
    Texture() = default;
    Texture(const std::string& filePath);


    void bind();
    void deleteTexture();


    unsigned int getID() const { return m_textureID; }

private:
    unsigned int m_textureID;
};