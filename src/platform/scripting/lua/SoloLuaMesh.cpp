#include "SoloLuaMesh.h"
#include "SoloMesh.h"
#include "SoloIndexedMeshPart.h"

using namespace solo;


IndexedMeshPart *LuaMesh::addPart(Mesh* mesh)
{
    return mesh->addPart(MeshIndexFormat::UnsignedShort);
}


void LuaMesh::resetVertexData(Mesh *mesh, unsigned storageId, const std::vector<float>& data, unsigned elementCount, bool dynamic)
{
    mesh->resetStorage(storageId, data.data(), elementCount, dynamic);
}


void LuaMesh::updateVertexData(Mesh *mesh, unsigned storageId, const std::vector<float>& data, unsigned elementCount, unsigned updateFromIndex)
{
    mesh->updateStorage(storageId, data.data(), elementCount, updateFromIndex);
}
