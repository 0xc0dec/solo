#pragma once

#include "SoloBase.h"
#include "SoloVector3.h"
#include "SoloVector2.h"
#include "SoloVertexFormat.h"

namespace solo
{
    enum class DeviceMode;

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

    class IndexedMeshPart
    {
    public:
        SL_NONCLONABLE(IndexedMeshPart);
        virtual ~IndexedMeshPart() {}

        virtual void resetIndexData(MeshIndexFormat indexFormat, float* data, unsigned elementCount, bool dynamic) = 0;
        virtual void updateIndexData(float *data, unsigned elementCount, unsigned updateFromIndex) = 0;

        virtual unsigned getElementCount() const = 0;
        virtual MeshIndexFormat getIndexFormat() const = 0;

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


    class Mesh2
    {
    public:
        static shared<Mesh2> create(DeviceMode mode);

        SL_NONCLONABLE(Mesh2);
        virtual ~Mesh2() {}

        virtual void resetVertexData(const VertexFormat &format, float *data, unsigned elementCount, bool dynamic) = 0;
        virtual void updateVertexData(float *data, unsigned elementCount, unsigned updateFromIndex) = 0;

        virtual IndexedMeshPart *addIndexedPart() = 0;
        virtual size_t getPartCount() const = 0;

        virtual void draw() = 0;
        virtual void drawIndexedPart(unsigned part) = 0;

        VertexFormat getVertexFormat() const;

        void setPrimitiveType(PrimitiveType type);
        PrimitiveType getPrimitiveType() const;

    protected:
        Mesh2() {}

        PrimitiveType primitiveType = PrimitiveType::Triangles;
        VertexFormat vertexFormat;
    };

    inline VertexFormat Mesh2::getVertexFormat() const
    {
        return vertexFormat;
    }

    inline void Mesh2::setPrimitiveType(PrimitiveType type)
    {
        primitiveType = type;
    }

    inline PrimitiveType Mesh2::getPrimitiveType() const
    {
        return primitiveType;
    }


    class Mesh
    {
    public:
        static shared<Mesh> create(DeviceMode mode);

        Mesh(const Mesh &other) = delete;
        Mesh(Mesh &&other) = delete;
        Mesh &operator=(const Mesh &other) = delete;
        Mesh &operator=(Mesh &&other) = delete;
        virtual ~Mesh() {}

        virtual void draw() = 0;

        virtual void setVertices(const std::vector<Vector3> &vertices) = 0;
        virtual void setNormals(const std::vector<Vector3> &normals) = 0;
        virtual void setUVs(const std::vector<Vector2> &uvs) = 0;
        virtual void setIndices(const std::vector<unsigned short> &indices) = 0;

        void rebuildAsQuad();
        void rebuildAsBox();
        // TODO other types of primitives

    protected:
        Mesh() {}
    };
}