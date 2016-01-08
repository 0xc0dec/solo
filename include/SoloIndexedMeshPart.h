#pragma once

#include "SoloBase.h"
#include "SoloMesh.h"

namespace solo
{
    enum class MeshIndexFormat
    {
        UnsignedByte,
        UnsignedShort,
        UnsignedInt
    };

    class IndexedMeshPart
    {
    public:
        SL_NONCOPYABLE(IndexedMeshPart);
        virtual ~IndexedMeshPart() {}

        virtual void resetIndexData(MeshIndexFormat indexFormat, const void* data, unsigned elementCount, bool dynamic) = 0;
        virtual void updateIndexData(const void *data, unsigned elementCount, unsigned updateFromIndex) = 0;

        void setPrimitiveType(MeshPrimitiveType type);
        MeshPrimitiveType getPrimitiveType() const;

    protected:
        IndexedMeshPart() {}

        MeshPrimitiveType primitiveType = MeshPrimitiveType::Triangles;
    };

    inline void IndexedMeshPart::setPrimitiveType(MeshPrimitiveType type)
    {
        primitiveType = type;
    }

    inline MeshPrimitiveType IndexedMeshPart::getPrimitiveType() const
    {
        return primitiveType;
    }
}