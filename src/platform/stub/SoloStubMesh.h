#pragma once

#include "SoloMesh.h"

namespace solo
{
    class StubMesh: public Mesh
    {
    public:
        virtual ~StubMesh() {}

        virtual void resetStorage(unsigned storageId, const float *data, unsigned elementCount, bool dynamic) override {}
        virtual void updateStorage(unsigned storageId, const float *data, unsigned elementCount, unsigned updateFromIndex) override {}

        virtual unsigned addIndex(MeshIndexFormat indexFormat) override;
        virtual void removeIndex(unsigned index) override;
        virtual size_t getIndexCount() const override;
        virtual void resetIndexData(unsigned index, const void *data, unsigned elementCount, bool dynamic) override {}
        virtual void updateIndexData(unsigned index, const void *data, unsigned elementCount, unsigned updateFromIndex) override {}

        virtual MeshPrimitiveType getIndexPrimitiveType(unsigned index) override;
        virtual void setIndexPrimitiveType(unsigned index, MeshPrimitiveType primitiveType) override;

        virtual void draw() override {}
        virtual void drawIndex(unsigned index) override {}

    private:
        friend class Mesh;

        StubMesh(const VertexFormat &format): Mesh(format) {}

        std::vector<unsigned> indexes;
        std::vector<MeshPrimitiveType> indexPrimitiveTypes;
    };

    inline size_t StubMesh::getIndexCount() const
    {
        return indexes.size();
    }

    inline MeshPrimitiveType StubMesh::getIndexPrimitiveType(unsigned index)
    {
        return indexPrimitiveTypes[index];
    }

    inline void StubMesh::setIndexPrimitiveType(unsigned index, MeshPrimitiveType primitiveType)
    {
        indexPrimitiveTypes[index] = primitiveType;
    }
}