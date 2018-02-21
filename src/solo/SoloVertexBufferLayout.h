/*
    Copyright (c) Aleksey Fedotov
    MIT license
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
        void addSemanticAttribute(VertexAttributeSemantics semantics);

        auto getAttributeCount() const -> u32 { return static_cast<u32>(attrs.size()); }
        auto getAttribute(u32 index) const -> VertexAttribute { return attrs.at(index); }

        auto getSize() const -> u32 { return size; }

    private:
        vec<VertexAttribute> attrs;
        u32 size = 0;

        void addAttribute(u32 elementCount, const str &name, VertexAttributeSemantics semantics);
    };

    
}
