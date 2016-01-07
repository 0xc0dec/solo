#pragma once

#include "SoloIndexedMeshPart.h"

namespace solo
{
    class StubIndexedMeshPart: public IndexedMeshPart
    {
    public:
        virtual ~StubIndexedMeshPart() {}

        virtual void resetIndexData(MeshIndexFormat indexFormat, float* data, unsigned elementCount, bool dynamic) override {}
        virtual void updateIndexData(float* data, unsigned elementCount, unsigned updateFromIndex) override {}

        virtual unsigned getElementCount() const override
        {
            return 0;
        }

        virtual MeshIndexFormat getIndexFormat() const override
        {
            return MeshIndexFormat();
        }

    private:
        friend class StubMesh;

        StubIndexedMeshPart() {}
    };
}