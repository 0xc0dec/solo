#pragma once

#include "SoloMesh.h"

namespace solo
{
    class StubMesh: public Mesh
    {
    public:
        virtual ~StubMesh() {}

        virtual void resetVertexData(const VertexFormat& format, float* data, unsigned elementCount, bool dynamic) override
        {
        }

        virtual void updateVertexData(float* data, unsigned elementCount, unsigned updateFromIndex) override
        {
        }

        IndexedMeshPart* addIndexedPart() override
        {
            return nullptr;
        }

        size_t getPartCount() const override
        {
            return 0;
        }

        void draw() override {}
        void drawIndexedPart(unsigned part) override {}

    private:
        friend class Mesh;

        StubMesh() {}
    };
}