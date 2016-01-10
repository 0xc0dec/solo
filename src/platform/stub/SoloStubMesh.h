#pragma once

#include "SoloMesh.h"

namespace solo
{
    class StubIndexedMeshPart;

    class StubMesh: public Mesh
    {
    public:
        virtual ~StubMesh() {}

        virtual void resetStorage(unsigned storageId, const float *data, unsigned elementCount, bool dynamic) override {}
        virtual void updateStorage(unsigned storageId, const float *data, unsigned elementCount, unsigned updateFromIndex) override {}

        virtual IndexedMeshPart *addPart(MeshIndexFormat indexFormat) override;
        virtual size_t getPartCount() const override;
        virtual IndexedMeshPart *getPart(unsigned index) const override;

        virtual void draw() override {}
        virtual void drawPart(unsigned part) override {}

    private:
        friend class Mesh;

        StubMesh(const VertexFormat &format): Mesh(format) {}

        std::vector<shared<StubIndexedMeshPart>> parts;
    };

    inline size_t StubMesh::getPartCount() const
    {
        return parts.size();
    }
}