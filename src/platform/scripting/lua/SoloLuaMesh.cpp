#include "SoloLuaMesh.h"
#include "SoloMesh.h"

using namespace solo;


void LuaMesh::resetIndexData(Mesh* mesh, int index, const std::vector<uint16_t>& data, int elementCount, bool dynamic)
{
    mesh->resetIndexData(index, data.data(), elementCount, dynamic);
}


void LuaMesh::updateIndexData(Mesh* mesh, int index, const std::vector<uint16_t>& data, int elementCount, int updateFromIndex)
{
    mesh->updateIndexData(index, data.data(), elementCount, updateFromIndex);
}


int LuaMesh::addIndex(Mesh* mesh)
{
    return mesh->addIndex(MeshIndexFormat::UnsignedShort);
}


void LuaMesh::resetData(Mesh* mesh, int storageId, const std::vector<float>& data, int elementCount, bool dynamic)
{
    mesh->resetData(storageId, data.data(), elementCount, dynamic);
}


void LuaMesh::updateData(Mesh* mesh, int storageId, const std::vector<float>& data, int elementCount, int updateFromIndex)
{
    mesh->updateData(storageId, data.data(), elementCount, updateFromIndex);
}
