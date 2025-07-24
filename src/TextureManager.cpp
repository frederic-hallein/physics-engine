#include "TextureManager.hpp"

void TextureManager::addTextures(std::vector<std::unique_ptr<Texture>> textures)
{
    for (auto& texture : textures)
    {
        m_textures[texture->getName()] = std::move(texture);
    }
}

Texture& TextureManager::getTexture(const std::string& name)
{
    auto it = m_textures.find(name);
    if (it == m_textures.end())
    {
        throw std::runtime_error("TextureManager: Texture '" + name + "' not found!");
    }
    return *(it->second);
}

void TextureManager::deleteAllTextures()
{
    for (auto& [name, texture] : m_textures)
    {
        texture->deleteTexture();
    }
}