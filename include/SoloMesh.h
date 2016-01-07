#pragma once

#include "SoloBase.h"
#include "SoloVertexFormat.h"

namespace solo
{
    enum class DeviceMode;
    class IndexedMeshPart;

    enum class PrimitiveType
    {
        Triangles,
        TriangleStrip,
        Lines,
        LineStrip,
        Points
    };

    enum class MeshIndexFormat
    {
        UnsignedByte,
        UnsignedShort,
        UnsignedInt
    };
    
    class Mesh
    {
    public:
        static shared<Mesh> create(DeviceMode mode);

        SL_NONCOPYABLE(Mesh);
        virtual ~Mesh() {}

        virtual void resetVertexData(const VertexFormat &format, float *data, unsigned elementCount, bool dynamic) = 0;
        virtual void updateVertexData(float *data, unsigned elementCount, unsigned updateFromIndex) = 0;

        virtual IndexedMeshPart *addIndexedPart() = 0;
        virtual size_t getPartCount() const = 0;

        virtual void draw() = 0;
        virtual void drawIndexedPart(unsigned part) = 0;

        VertexFormat getVertexFormat() const;

        void setPrimitiveType(PrimitiveType type);
        PrimitiveType getPrimitiveType() const;

        void rebuildAsQuad() {} // TODO
        void rebuildAsBox() {} // TODO
        // TODO other types of primitives

    protected:
        Mesh() {}

        PrimitiveType primitiveType = PrimitiveType::Triangles;
        VertexFormat vertexFormat;
    };

    inline VertexFormat Mesh::getVertexFormat() const
    {
        return vertexFormat;
    }

    inline void Mesh::setPrimitiveType(PrimitiveType type)
    {
        primitiveType = type;
    }

    inline PrimitiveType Mesh::getPrimitiveType() const
    {
        return primitiveType;
    }
}