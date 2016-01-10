#pragma once

#include "SoloBase.h"
#include "SoloVertexFormat.h"

namespace solo
{
    enum class DeviceMode;
    enum class MeshIndexFormat;
    class IndexedMeshPart;

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

    class Mesh
    {
    public:
        static shared<Mesh> create(DeviceMode mode, const VertexFormat &vertexFormat);
        static shared<Mesh> createPrefab(DeviceMode mode, MeshPrefab prefab);

        SL_NONCOPYABLE(Mesh);
        virtual ~Mesh() {}

        virtual void resetStorage(unsigned storageId, const float *data, unsigned elementCount, bool dynamic) = 0;
        virtual void updateStorage(unsigned storageId, const float *data, unsigned elementCount, unsigned updateFromIndex) = 0;

        virtual IndexedMeshPart *addPart(MeshIndexFormat indexFormat) = 0;
        virtual size_t getPartCount() const = 0;
        virtual IndexedMeshPart *getPart(unsigned index) const = 0;

        virtual void draw() = 0;
        virtual void drawPart(unsigned part) = 0;

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
