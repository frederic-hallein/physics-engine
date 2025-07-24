#include "MeshManager.hpp"

void MeshManager::addMeshes(std::vector<std::unique_ptr<Mesh>> meshes)
{
    for (auto& mesh : meshes)
    {
        m_meshes[mesh->getName()] = std::move(mesh);
    }
}

Mesh& MeshManager::getMesh(const std::string& name)
{
    auto it = m_meshes.find(name);
    if (it == m_meshes.end())
    {
        throw std::runtime_error("MeshManager: Mesh '" + name + "' not found!");
    }
    return *(it->second);
}

void MeshManager::deleteAllMeshes()
{
    for (auto& [name, mesh] : m_meshes)
    {
        mesh->deleteMesh();
    }
}