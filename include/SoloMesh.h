#pragma once

#include "SoloBase.h"
#include "SoloVertexFormat.h"

namespace solo
{
    enum class DeviceMode;
    enum class MeshIndexFormat;

    enum class MeshPrimitiveType
    {
        Triangles,
        TriangleStrip,
        Lines,
        LineStrip,
        Points
    };

    enum class MeshPrefab
    {
        Quad,
        Cube
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
        static shared<Mesh> create(DeviceMode mode, const VertexFormat &vertexFormat);
        static shared<Mesh> createPrefab(DeviceMode mode, MeshPrefab prefab);

        SL_NONCOPYABLE(Mesh);
        virtual ~Mesh() {}

        virtual void resetStorage(unsigned storageId, const float *data, unsigned elementCount, bool dynamic) = 0;
        virtual void updateStorage(unsigned storageId, const float *data, unsigned elementCount, unsigned updateFromIndex) = 0;

        virtual unsigned addIndex(MeshIndexFormat indexFormat) = 0;
        virtual void removeIndex(unsigned index) = 0;
        virtual size_t getIndexCount() const = 0;

        virtual void resetIndexData(unsigned index, const void *data, unsigned elementCount, bool dynamic) = 0;
        virtual void updateIndexData(unsigned index, const void *data, unsigned elementCount, unsigned updateFromIndex) = 0;

        virtual MeshPrimitiveType getIndexPrimitiveType(unsigned index) = 0;
        virtual void setIndexPrimitiveType(unsigned index, MeshPrimitiveType primitiveType) = 0;

        virtual void draw() = 0;
        virtual void drawIndex(unsigned index) = 0;

        VertexFormat getVertexFormat() const;

        void setPrimitiveType(MeshPrimitiveType type);
        MeshPrimitiveType getPrimitiveType() const;

    protected:
        Mesh(const VertexFormat &vertexFormat): vertexFormat(vertexFormat) {}

        static shared<Mesh> createQuadMesh(DeviceMode mode);
        static shared<Mesh> createBoxMesh(DeviceMode mode);

        MeshPrimitiveType primitiveType = MeshPrimitiveType::Triangles;
        VertexFormat vertexFormat;
    };

    inline VertexFormat Mesh::getVertexFormat() const
    {
        return vertexFormat;
    }

    inline void Mesh::setPrimitiveType(MeshPrimitiveType type)
    {
        primitiveType = type;
    }

    inline MeshPrimitiveType Mesh::getPrimitiveType() const
    {
        return primitiveType;
    }
}
