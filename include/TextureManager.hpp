#pragma once

#include <unordered_map>
#include <memory>
#include <string>

#include "Texture.hpp"

class TextureManager
{
public:

    void addTexture(const std::string& name, const std::string& texturePath);
    Texture* getTexture(const std::string& name) { return textures[name].get(); }
    void bindTexture(const std::string& name) { textures[name]->bind(); };
    void deleteAllTextures();

private:
    std::unordered_map<std::string, std::unique_ptr<Texture>> textures;
};