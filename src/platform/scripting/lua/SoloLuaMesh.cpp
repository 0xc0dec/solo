#include "SoloLuaMesh.h"
#include "SoloMesh.h"

using namespace solo;


void LuaMesh::addBuffer(Mesh* mesh, const VertexBufferLayout& layout, const std::vector<float>& data, int elementCount)
{
    mesh->addBuffer(layout, data.data(), elementCount);
}


void LuaMesh::addPart(Mesh* mesh, const std::vector<float>& data, int elementCount)
{
    mesh->addPart(data.data(), elementCount);
}
