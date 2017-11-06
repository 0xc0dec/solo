/*
    Copyright (c) Aleksey Fedotov
    MIT license
*/

#pragma once

#include "SoloCommon.h"
#include "SoloVertexFormat.h"
#include "SoloRenderer.h"
#include "SoloAsyncHandle.h"

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
        static auto loadFromFile(Device *device, const str &path) -> sptr<Mesh>;
        static auto loadFromFileAsync(Device *device, const str &path) -> sptr<AsyncHandle<Mesh>>;

        virtual ~Mesh() {}

        virtual auto addVertexBuffer(const VertexBufferLayout &layout, const void *data, u32 vertexCount) -> u32 = 0;
        virtual auto addDynamicVertexBuffer(const VertexBufferLayout &layout, const void *data, u32 vertexCount) -> u32 = 0;
        virtual void updateDynamicVertexBuffer(u32 index, u32 vertexOffset, const void *data, u32 vertexCount) = 0;
        virtual void removeVertexBuffer(u32 index) = 0;

        virtual auto addPart(const void *indexData, u32 indexElementCount) -> u32 = 0;
        virtual void removePart(u32 index) = 0;
        virtual auto getPartCount() const -> u32 = 0;

        virtual auto getPrimitiveType() const -> PrimitiveType = 0;
        virtual void setPrimitiveType(PrimitiveType type) = 0;

    protected:
        Mesh() {}

        void initAsQuadMesh();
        void initAsCubeMesh();
    };
}
