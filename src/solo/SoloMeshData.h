/* 
 * Copyright (c) Aleksey Fedotov 
 * MIT license 
 */

#pragma once

#include "SoloCommon.h"
#include "SoloAsyncHandle.h"

namespace solo
{
    class Device;
    class VertexBufferLayout;
	enum class IndexElementSize;

    class MeshData
    {
    public:
        static auto fromFile(Device *device, const str &path, const VertexBufferLayout &bufferLayout) -> sptr<MeshData>;
        static auto fromFileAsync(Device *device, const str &path, const VertexBufferLayout &bufferLayout) -> sptr<AsyncHandle<MeshData>>;

        auto vertexData() const -> const vec<float>& { return vertexData_; }
        auto vertexCount() const -> u32 { return vertexCount_; }
    	auto partsCount() const -> u32 { return indexData_.size(); }
    	auto indexData(u32 part) const -> const vec<u32>& { return indexData_.at(part); }
    	auto indexElementCount(u32 part) const -> u32 { return indexData_.at(part).size(); }

    private:
        vec<float> vertexData_;
        u32 vertexCount_ = 0;
        vec<vec<u32>> indexData_;
    };
}
