#pragma once

#include "SoloBase.h"
#include "SoloMesh.h"

namespace solo
{
    class IndexedMeshPart
    {
    public:
        SL_NONCOPYABLE(IndexedMeshPart);
        virtual ~IndexedMeshPart() {}

        virtual void resetIndexData(MeshIndexFormat indexFormat, float* data, unsigned elementCount, bool dynamic) = 0;
        virtual void updateIndexData(float *data, unsigned elementCount, unsigned updateFromIndex) = 0;

        void setPrimitiveType(PrimitiveType type);
        PrimitiveType getPrimitiveType() const;

    protected:
        IndexedMeshPart() {}

        PrimitiveType primitiveType = PrimitiveType::Triangles;
    };

    inline void IndexedMeshPart::setPrimitiveType(PrimitiveType type)
    {
        primitiveType = type;
    }

    inline PrimitiveType IndexedMeshPart::getPrimitiveType() const
    {
        return primitiveType;
    }
}