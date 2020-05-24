/* 
 * Copyright (c) Aleksey Fedotov 
 * MIT license 
 */

#pragma once

#include "SoloCommon.h"

namespace solo
{
    enum class VertexAttributeUsage
    {
        Unknown,
        Position,
        Normal,
        TexCoord,
        Tangent,
        Binormal
    };

    class VertexAttribute final
    {
    public:
        str name;
        u32 elementCount;
        u32 size;
        u32 location;
        u32 offset;
        VertexAttributeUsage usage;
    };

    class VertexBufferLayout final
    {
    public:
        void addAttribute(VertexAttributeUsage usage);

        auto attributeCount() const -> u32 { return static_cast<u32>(attributes_.size()); }
        auto attribute(u32 index) const -> VertexAttribute { return attributes_.at(index); }
    	auto attributeIndex(VertexAttributeUsage usage) const -> s32;

        auto size() const -> u32 { return size_; }
        auto elementCount() const -> u32 { return elementCount_; }

    private:
        vec<VertexAttribute> attributes_;
        u32 size_ = 0; // in bytes
        u32 elementCount_ = 0; // number of "elements" (floats)

        void addAttribute(u32 elementCount, const str &name, VertexAttributeUsage usage);
    };
}
