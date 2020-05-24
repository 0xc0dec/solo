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

    	// TODO rename to addVertexBuffer and add a 'dynamic' flag
        virtual auto addVertexBuffer(const VertexBufferLayout &layout, const vec<float> &data, u32 vertexCount) -> u32;
        virtual auto addDynamicVertexBuffer(const VertexBufferLayout &layout, const vec<float> &data, u32 vertexCount) -> u32;
    	// TODO rename to updateVertexBuffer
        virtual void updateDynamicVertexBuffer(u32 index, u32 vertexOffset, const void *data, u32 vertexCount);
        virtual void removeVertexBuffer(u32 index);
    	auto vertexBufferCount() const -> u32 { return layouts_.size(); }
    	auto vertexBufferVertexCount(u32 index) const -> u32 { return vertexCounts_.at(index); }
    	auto vertexBufferLayout(u32 index) const -> VertexBufferLayout { return layouts_.at(index); }
    	auto vertexBufferData(u32 index) const -> const vec<float>& { return vertexData_.at(index); }

        virtual auto addPart(const vec<u32> &data, u32 indexElementCount) -> u32;
        virtual void removePart(u32 part);
        auto partCount() const -> u32 { return static_cast<u32>(indexElementCounts_.size()); }
    	auto partElementCount(u32 part) const -> u32 { return indexElementCounts_.at(part); }
    	auto partElementSize(u32 part) const -> IndexElementSize { return IndexElementSize::Bits32; } // TODO 16-bit support?
    	auto partData(u32 part) const -> const vec<u32>& { return partData_.at(part); }

        virtual auto primitiveType() const -> PrimitiveType = 0;
        virtual void setPrimitiveType(PrimitiveType type) = 0;

    protected:
		vec<VertexBufferLayout> layouts_;
    	vec<u32> indexElementCounts_;
    	u32 minVertexCount_ = 0;
    	
        Mesh() = default;

    private:
    	vec<vec<float>> vertexData_;
    	vec<vec<u32>> partData_;
    	vec<u32> vertexCounts_;
    	
    	void updateMinVertexCount();
    };
}
