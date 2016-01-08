#include "SoloLuaMesh.h"
#include "SoloMesh.h"

using namespace solo;


void LuaMesh::resetVertexData(Mesh *mesh, const VertexFormat& format, const std::vector<float>& data, unsigned elementCount, bool dynamic)
{
    mesh->resetVertexData(format, data.data(), elementCount, dynamic);
}


void LuaMesh::updateVertexData(Mesh *mesh, const std::vector<float>& data, unsigned elementCount, unsigned updateFromIndex)
{
    mesh->updateVertexData(data.data(), elementCount, updateFromIndex);
}
