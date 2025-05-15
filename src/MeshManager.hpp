#pragma once

#include <unordered_map>
#include <memory>
#include <string>

#include "Mesh.hpp"

class MeshManager
{
public:

    void addMesh(std::unique_ptr<Mesh> mesh);
    Mesh& getMesh(const std::string& name) { return *meshes[name]; }
    void deleteAllMeshes();

private:
    std::unordered_map<std::string, std::unique_ptr<Mesh>> meshes;
};