#include "SoloStubMesh.h"
#include "SoloStubIndexedMeshPart.h"

using namespace solo;


IndexedMeshPart* StubMesh::addIndexedPart()
{
    auto part = SL_NEW_SHARED(StubIndexedMeshPart);
    parts.push_back(part);
    return part.get();
}