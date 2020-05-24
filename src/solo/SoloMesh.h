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
    enum class PrimitiveType
    {
        Triangles,
        TriangleStrip,
        Lines,
        LineStrip,
        Points
    };

	enum class IndexElementSize
	{
		Bits16 = sizeof(u16),
		Bits32 = sizeof(u32)
	};

    class Device;

    class Mesh: public NoCopyAndMove
    {
    public:
        static auto empty(Device *device) -> sptr<Mesh>;
        static auto fromFile(Device *device, const str &path, const VertexBufferLayout &bufferLayout) -> sptr<Mesh>;
        static auto fromFileAsync(Device *device, const str &path, const VertexBufferLayout &bufferLayout) -> sptr<AsyncHandle<Mesh>>;

        virtual ~Mesh() = default;

        virtual auto addVertexBuffer(const VertexBufferLayout &layout, const void *data, u32 vertexCount) -> u32;
        virtual auto addDynamicVertexBuffer(const VertexBufferLayout &layout, const void *data, u32 vertexCount) -> u32;
        virtual void updateDynamicVertexBuffer(u32 index, u32 vertexOffset, const void *data, u32 vertexCount);
        virtual void removeVertexBuffer(u32 index);

    	// TODO Forbid setting different element size for different parts?
        virtual auto addPart(const void *indexData, u32 indexElementCount, IndexElementSize elementSize) -> u32;
        virtual void removePart(u32 part);
        auto partCount() const -> u32 { return static_cast<u32>(indexElementCounts_.size()); }
    	auto partElementSize(u32 part) -> IndexElementSize { return indexElementSizes_.at(part); }

        virtual auto primitiveType() const -> PrimitiveType = 0;
        virtual void setPrimitiveType(PrimitiveType type) = 0;

    protected:
		vec<VertexBufferLayout> layouts_;
    	vec<u32> vertexCounts_;
    	vec<u32> indexElementCounts_;
    	vec<IndexElementSize> indexElementSizes_;
    	u32 minVertexCount_ = 0;
    	
        Mesh() = default;

    private:
    	void updateMinVertexCount();
    };
}
