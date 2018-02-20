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
        Position,
        Normal,
        TexCoord
    };

    class VertexAttribute final
    {
    public:
        str name;
        u32 elementCount;
        u32 size;
        u32 location;
        u32 offset;
    };

    class VertexBufferLayout final
    {
    public:
        void addNamedAttribute(u32 elementCount, const str &name);
        void addSemanticAttribute(VertexAttributeSemantics semantics);

        auto getAttributeCount() const -> u32 { return static_cast<u32>(attrs.size()); }
        auto getAttribute(u32 index) const -> VertexAttribute { return attrs.at(index); }

        auto getSize() const -> u32 { return size; }

    private:
        vec<VertexAttribute> attrs;
        u32 size = 0;
    };

    
}
