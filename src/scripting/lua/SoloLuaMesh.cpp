#include "SoloLuaMesh.h"
#include "SoloMesh.h"

using namespace solo;


uint32_t LuaMesh::addBuffer(Mesh* mesh, const VertexBufferLayout& layout, const std::vector<float>& data, uint32_t vertexCount)
{
    return mesh->addBuffer(layout, data.data(), vertexCount);
}


uint32_t LuaMesh::addDynamicBuffer(Mesh* mesh, const VertexBufferLayout& layout, const std::vector<float>& data, uint32_t vertexCount)
{
    return mesh->addDynamicBuffer(layout, data.data(), vertexCount);
}


void LuaMesh::updateDynamicBuffer(Mesh* mesh, uint32_t index, uint32_t offset, const std::vector<float>& data, uint32_t vertexCount)
{
    mesh->updateDynamicBuffer(index, offset, data.data(), vertexCount);
}


uint32_t LuaMesh::addPart(Mesh* mesh, const std::vector<float>& data, uint32_t elementCount)
{
    return mesh->addPart(data.data(), elementCount);
}
