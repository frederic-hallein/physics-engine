#include "MeshManager.hpp"

void MeshManager::addMesh(std::unique_ptr<Mesh> mesh)
{
    meshes[mesh->getName()] = std::move(mesh);
}

void MeshManager::deleteAllMeshes()
{
    for (auto& [name, mesh] : meshes)
    {
        mesh->deleteMesh();
    }
}