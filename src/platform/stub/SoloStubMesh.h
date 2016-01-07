#pragma once

#include "SoloMesh.h"

namespace solo
{
    class StubIndexedMeshPart;

    class StubMesh: public Mesh
    {
    public:
        virtual ~StubMesh() {}

        virtual void resetVertexData(const VertexFormat& format, float* data, unsigned elementCount, bool dynamic) override {}
        virtual void updateVertexData(float* data, unsigned elementCount, unsigned updateFromIndex) override {}

        virtual IndexedMeshPart* addIndexedPart() override;

        virtual size_t getPartCount() const override;

        virtual void draw() override {}
        virtual void drawIndexedPart(unsigned part) override {}

    private:
        friend class Mesh;

        StubMesh() {}

        std::vector<shared<StubIndexedMeshPart>> parts;
    };

    inline size_t StubMesh::getPartCount() const
    {
        return parts.size();
    }
}