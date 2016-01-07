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

    protected:
        IndexedMeshPart() {}
    };


    class Mesh2
    {
    public:
        static shared<Mesh2> create(DeviceMode mode);

        SL_NONCLONABLE(Mesh2);
        virtual ~Mesh2() {}

    protected:
        Mesh2() {}
    };


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