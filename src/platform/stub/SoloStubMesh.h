#pragma once

#include "SoloMesh.h"

namespace solo
{
    class StubMesh: public Mesh
    {
    public:
        virtual ~StubMesh() {}

        virtual void resetData(int slot, const float* data, int elementCount, bool dynamic) override {}
        virtual void updateData(int slot, const float* data, int elementCount, int updateFromIndex) override {}

        virtual int addIndex(MeshIndexFormat indexFormat) override;
        virtual void removeIndex(int index) override;
        virtual int getIndexCount() const override;
        virtual void resetIndexData(int index, const void* data, int elementCount, bool dynamic) override {}
        virtual void updateIndexData(int index, const void* data, int elementCount, int updateFromIndex) override {}

        virtual MeshPrimitiveType getIndexPrimitiveType(int index) override;
        virtual void setIndexPrimitiveType(int index, MeshPrimitiveType primitiveType) override;

        virtual void draw() override {}
        virtual void drawIndex(int index) override {}

    private:
        friend class Mesh;

        StubMesh(const VertexFormat& format): Mesh(format) {}

        std::vector<int> indexes;
        std::vector<MeshPrimitiveType> indexPrimitiveTypes;
    };

    inline int StubMesh::getIndexCount() const
    {
        return static_cast<int>(indexes.size());
    }

    inline MeshPrimitiveType StubMesh::getIndexPrimitiveType(int index)
    {
        return indexPrimitiveTypes[index];
    }

    inline void StubMesh::setIndexPrimitiveType(int index, MeshPrimitiveType primitiveType)
    {
        indexPrimitiveTypes[index] = primitiveType;
    }
}