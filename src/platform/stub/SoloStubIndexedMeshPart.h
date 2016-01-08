#pragma once

#include "SoloIndexedMeshPart.h"

namespace solo
{
    class StubIndexedMeshPart: public IndexedMeshPart
    {
    public:
        virtual ~StubIndexedMeshPart() {}

        virtual void resetIndexData(const void *data, unsigned elementCount, bool dynamic) override {}
        virtual void updateIndexData(const void *data, unsigned elementCount, unsigned updateFromIndex) override {}

    private:
        friend class StubMesh;

        StubIndexedMeshPart(MeshIndexFormat indexFormat): IndexedMeshPart(indexFormat) {}
    };
}