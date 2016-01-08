#include "SoloStubMesh.h"
#include "SoloStubIndexedMeshPart.h"

using namespace solo;


IndexedMeshPart* StubMesh::addPart()
{
    auto part = SL_NEW_SHARED(StubIndexedMeshPart);
    parts.push_back(part);
    return part.get();
}


IndexedMeshPart* StubMesh::getPart(unsigned index) const
{
    return parts[index].get();
}
