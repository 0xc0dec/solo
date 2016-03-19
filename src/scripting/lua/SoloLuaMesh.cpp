#include "SoloLuaMesh.h"
#include "SoloMesh.h"

using namespace solo;


uint32_t LuaMesh::addVertexBuffer(Mesh* mesh, const VertexBufferLayout& layout, const std::vector<float>& data, uint32_t vertexCount)
{
    return mesh->addVertexBuffer(layout, data.data(), vertexCount);
}


uint32_t LuaMesh::addDynamicVertexBuffer(Mesh* mesh, const VertexBufferLayout& layout, const std::vector<float>& data, uint32_t vertexCount)
{
    return mesh->addDynamicVertexBuffer(layout, data.data(), vertexCount);
}


void LuaMesh::updateDynamicVertexBuffer(Mesh* mesh, uint32_t index, uint32_t vertexOffset, const std::vector<float>& data, uint32_t vertexCount)
{
    mesh->updateDynamicVertexBuffer(index, vertexOffset, data.data(), vertexCount);
}


uint32_t LuaMesh::addPart(Mesh* mesh, const std::vector<uint16_t>& data, uint32_t elementCount)
{
    return mesh->addPart(data.data(), elementCount);
}
