/*
    Copyright (c) Aleksey Fedotov
    MIT license
*/

#pragma once

#include "SoloCommon.h"

namespace solo
{
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
        // TODO "addPrefabAttribute" with "semantics", which simply translates "position" into 0, "normal" into 1, etc.
        void addAttribute(u32 elementCount, u32 location);
        void addNamedAttribute(u32 elementCount, const str &name);

        auto getAttributeCount() const -> u32 { return static_cast<u32>(attrs.size()); }
        auto getAttribute(u32 index) const -> VertexAttribute { return attrs.at(index); }

        auto getSize() const -> u32 { return size; }

    private:
        vec<VertexAttribute> attrs;
        u32 size = 0;
    };

    inline void VertexBufferLayout::addAttribute(u32 elementCount, u32 location)
    {
        const auto size = static_cast<u32>(sizeof(float) * elementCount);
        const auto offset = attrs.empty() ? 0 : attrs.crbegin()->offset + attrs.crbegin()->size;
        attrs.push_back(VertexAttribute{"", elementCount, size, location, offset});
        this->size += size;
    }

    inline void VertexBufferLayout::addNamedAttribute(u32 elementCount, const str &name)
    {
        const auto size = static_cast<u32>(sizeof(float) * elementCount);
        const auto offset = attrs.empty() ? 0 : attrs.crbegin()->offset + attrs.crbegin()->size;
        attrs.push_back(VertexAttribute{name, elementCount, size, 0, offset});
        this->size += size;
    }
}
