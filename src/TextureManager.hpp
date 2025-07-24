#pragma once

#include <unordered_map>
#include <memory>
#include <string>

#include "Texture.hpp"

class TextureManager
{
public:
    void addTextures(std::vector<std::unique_ptr<Texture>> textures);
    Texture& getTexture(const std::string& name);
    void deleteAllTextures();

private:
    std::unordered_map<std::string, std::unique_ptr<Texture>> m_textures;
};