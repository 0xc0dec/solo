/* 
 * Copyright (c) Aleksey Fedotov 
 * MIT license 
 */

#pragma once

#include "SoloCommon.h"

namespace solo
{
    enum class VertexAttributeSemantics
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
        VertexAttributeSemantics semantics;
    };

    class VertexBufferLayout final
    {
    public:
        void addAttribute(VertexAttributeSemantics semantics);

        auto attributeCount() const -> u32 { return static_cast<u32>(attrs_.size()); }
        auto attribute(u32 index) const -> VertexAttribute { return attrs_.at(index); }

        auto size() const -> u32 { return size_; }
        auto elementCount() const -> u32 { return elementCount_; }

    private:
        vec<VertexAttribute> attrs_;
        u32 size_ = 0; // in bytes
        u32 elementCount_ = 0; // number of floats

        void addAttribute(u32 elementCount, const str &name, VertexAttributeSemantics semantics);
    };
}
