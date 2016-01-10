#include "SoloStubMesh.h"
#include "SoloStubIndexedMeshPart.h"

using namespace solo;


IndexedMeshPart *StubMesh::addPart(MeshIndexFormat indexFormat)
{
    auto part = SL_NEW_SHARED(StubIndexedMeshPart, indexFormat);
    parts.push_back(part);
    return part.get();
}


IndexedMeshPart *StubMesh::getPart(unsigned index) const
{
    return parts[index].get();
}
