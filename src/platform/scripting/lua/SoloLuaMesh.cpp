#include "SoloLuaMesh.h"
#include "SoloMesh.h"

using namespace solo;


void LuaMesh::resetIndexData(Mesh* mesh, unsigned index, const std::vector<uint16_t>& data, unsigned elementCount, bool dynamic)
{
    mesh->resetIndexData(index, data.data(), elementCount, dynamic);
}


void LuaMesh::updateIndexData(Mesh* mesh, unsigned index, const std::vector<uint16_t>& data, unsigned elementCount, unsigned updateFromIndex)
{
    mesh->updateIndexData(index, data.data(), elementCount, updateFromIndex);
}


unsigned LuaMesh::addIndex(Mesh* mesh)
{
    return mesh->addIndex(MeshIndexFormat::UnsignedShort);
}


void LuaMesh::resetStorage(Mesh* mesh, unsigned storageId, const std::vector<float>& data, unsigned elementCount, bool dynamic)
{
    mesh->resetStorage(storageId, data.data(), elementCount, dynamic);
}


void LuaMesh::updateStorage(Mesh* mesh, unsigned storageId, const std::vector<float>& data, unsigned elementCount, unsigned updateFromIndex)
{
    mesh->updateStorage(storageId, data.data(), elementCount, updateFromIndex);
}
