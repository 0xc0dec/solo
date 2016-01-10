#include "SoloStubMesh.h"

using namespace solo;


unsigned StubMesh::addIndex(MeshIndexFormat indexFormat)
{
    indexes.push_back(indexes.size());
    indexPrimitiveTypes.push_back(MeshPrimitiveType::Triangles);
    return indexes.size() - 1;
}


void StubMesh::removeIndex(unsigned index)
{
    indexes.erase(indexes.begin() + index);
    indexPrimitiveTypes.erase(indexPrimitiveTypes.begin() + index);
}
