#include "TextureManager.hpp"

void TextureManager::addTexture(std::unique_ptr<Texture> texture)
{
    textures[texture->getName()] = std::move(texture);
}

void TextureManager::deleteAllTextures()
{
    for (auto& [name, texture] : textures)
    {
        texture->deleteTexture();
    }
}