/*
 * Copyright (c) Aleksey Fedotov
 * MIT license
 */

#pragma once

#include "SoloCommon.h"
#include "SoloVertexBufferLayout.h"
#include "SoloAsyncHandle.h"

namespace solo
{
    class Device;

    // TODO Support for "non-GPU" meshes
    class Mesh
    {
    public:
        static auto empty(Device *device) -> sptr<Mesh>;
        static auto fromFile(Device *device, const str &path, const VertexBufferLayout &bufferLayout) -> sptr<Mesh>;
        static auto fromFileAsync(Device *device, const str &path, const VertexBufferLayout &bufferLayout) -> sptr<AsyncHandle<Mesh>>;

        Mesh(const Mesh &other) = delete;
        Mesh(Mesh &&other) = delete;
        virtual ~Mesh() = default;

        auto operator=(const Mesh &other) -> Mesh & = delete;
        auto operator=(Mesh &&other) -> Mesh & = delete;

        virtual auto addVertexBuffer(const VertexBufferLayout &layout, const vec<float> &data, u32 vertexCount) -> u32;
        virtual auto addDynamicVertexBuffer(const VertexBufferLayout &layout, const vec<float> &data, u32 vertexCount) -> u32;
        virtual void updateVertexBuffer(u32 index, u32 vertexOffset, const void *data, u32 vertexCount) {}
        virtual void removeVertexBuffer(u32 index);
        auto vertexBufferCount() const -> u32
        {
            return layouts_.size();
        }
        auto vertexBufferVertexCount(u32 index) const -> u32
        {
            return vertexCounts_.at(index);
        }
        auto vertexBufferLayout(u32 index) const -> VertexBufferLayout
        {
            return layouts_.at(index);
        }
        auto vertexBufferData(u32 index) const -> const vec<float> & { return vertexData_.at(index); }

        virtual auto addIndexBuffer(const vec<u32> &data, u32 elementCount) -> u32;
        virtual void removeIndexBuffer(u32 index);
        auto indexBufferCount() const -> u32
        {
            return static_cast<u32>(indexElementCounts_.size());
        }
        auto indexBufferElementCount(u32 index) const -> u32
        {
            return indexElementCounts_.at(index);
        }
        auto indexBufferElementSize(u32 index) const -> IndexElementSize
        {
            return IndexElementSize::Bits32;    // TODO 16-bit support?
        }
        auto indexData(u32 index) const -> const vec<u32> & { return indexData_.at(index); }

        auto primitiveType() const -> PrimitiveType
        {
            return primitiveType_;
        }
        void setPrimitiveType(PrimitiveType type)
        {
            primitiveType_ = type;
        }

    protected:
        PrimitiveType primitiveType_ = PrimitiveType::Triangles;
        vec<VertexBufferLayout> layouts_;
        vec<u32> indexElementCounts_;
        u32 minVertexCount_ = 0;

        Mesh() = default;

    private:
        vec<vec<float>> vertexData_;
        vec<vec<u32>> indexData_;
        vec<u32> vertexCounts_;

        void updateMinVertexCount();
    };
}
