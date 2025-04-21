#include "TextureManager.hpp"

void TextureManager::addTexture(const std::string& name, const std::string& texturePath)
{
    textures[name] = std::make_unique<Texture>(texturePath.c_str());
}

void TextureManager::deleteAllTextures()
{
    for (auto& [name, texture] : textures)
    {
        texture->deleteTexture();
    }
}