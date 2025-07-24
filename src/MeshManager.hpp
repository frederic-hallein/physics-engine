#pragma once

#include <unordered_map>
#include <memory>
#include <string>

#include "Mesh.hpp"

class MeshManager
{
public:
    void addMeshes(std::vector<std::unique_ptr<Mesh>> meshes);
    Mesh& getMesh(const std::string& name);
    void deleteAllMeshes();

private:
    std::unordered_map<std::string, std::unique_ptr<Mesh>> m_meshes;
};