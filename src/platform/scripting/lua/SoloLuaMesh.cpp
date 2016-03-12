#include "SoloLuaMesh.h"
#include "SoloMesh.h"

using namespace solo;


void LuaMesh::addBuffer(Mesh* mesh, const VertexBufferLayout& layout, const std::vector<float>& data, int elementCount)
{
    mesh->addBuffer(layout, data.data(), elementCount);
}


void LuaMesh::addIndex(Mesh* mesh, const std::vector<float>& data, int elementCount)
{
    mesh->addIndex(data.data(), elementCount);
}
