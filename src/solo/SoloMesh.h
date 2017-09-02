/*
    Copyright (c) Aleksey Fedotov
    MIT license
*/

#pragma once

#include "SoloCommon.h"
#include "SoloVertexFormat.h"
#include "SoloRenderer.h"

namespace solo
{
    enum class PrimitiveType
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

    class Device;

    class Mesh
    {
    public:
        SL_DISABLE_COPY_AND_MOVE(Mesh)

        static auto create(Device *device) -> sptr<Mesh>;
        static auto createFromPrefab(Device *device, MeshPrefab prefab) -> sptr<Mesh>;
        static auto loadFromFile(Device *device, const std::string &path) -> sptr<Mesh>;

        virtual ~Mesh() {}

        virtual auto addVertexBuffer(const VertexBufferLayout &layout, const void *data, uint32_t vertexCount) -> uint32_t = 0;
        virtual auto addDynamicVertexBuffer(const VertexBufferLayout &layout, const void *data, uint32_t vertexCount) -> uint32_t = 0;
        virtual void updateDynamicVertexBuffer(uint32_t index, uint32_t vertexOffset, const void *data, uint32_t vertexCount) = 0;
        virtual void removeVertexBuffer(uint32_t index) = 0;

        virtual auto addPart(const void *indexData, uint32_t indexElementCount) -> uint32_t = 0;
        virtual void removePart(uint32_t index) = 0;
        virtual auto getPartCount() const -> uint32_t = 0;

        virtual auto getPrimitiveType() const -> PrimitiveType = 0;
        virtual void setPrimitiveType(PrimitiveType type) = 0;

    protected:
        Mesh() {}

        void initAsQuadMesh();
        void initAsCubeMesh();
    };
}
