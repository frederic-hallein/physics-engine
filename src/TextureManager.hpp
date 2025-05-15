#pragma once

#include <unordered_map>
#include <memory>
#include <string>

#include "Texture.hpp"

class TextureManager
{
public:

    void addTexture(std::unique_ptr<Texture> texture);
    Texture& getTexture(const std::string& name) { return *textures[name]; }
    void deleteAllTextures();

private:
    std::unordered_map<std::string, std::unique_ptr<Texture>> textures;
};