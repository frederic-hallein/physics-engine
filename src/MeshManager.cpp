#include "MeshManager.hpp"

void MeshManager::addMesh(const std::string& name, const std::string& meshPath)
{
    meshes[name] = std::make_unique<Mesh>(meshPath.c_str());
}

void MeshManager::deleteAllMeshes()
{
    for (auto& [name, mesh] : meshes)
    {
        mesh->deleteMesh();
    }
}