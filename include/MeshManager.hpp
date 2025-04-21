#pragma once

#include <unordered_map>
#include <memory>
#include <string>
#include "Mesh.hpp"

class MeshManager
{
public:

    void addMesh(const std::string& name, const std::string& meshPath);
    Mesh* getMesh(const std::string& name) { return meshes[name].get(); }
    void drawMesh(const std::string& name) { meshes[name]->draw(); };
    void deleteAllMeshes();

private:
    std::unordered_map<std::string, std::unique_ptr<Mesh>> meshes;
};