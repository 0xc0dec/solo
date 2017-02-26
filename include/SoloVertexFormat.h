/*
    Copyright (c) Aleksey Fedotov
    MIT license
*/

#pragma once

#include <vector>


namespace solo
{
    class VertexAttribute final
    {
    public:
        uint32_t elementCount;
        uint32_t size;
        uint32_t location;
    };

    class VertexBufferLayout final
    {
    public:
        // TODO "addPrefabAttribute" with "semantics", which simply translates "position" into 0, "normal" into 1, etc.
        void addAttribute(uint32_t elementCount, uint32_t location);

        auto getAttributeCount() const -> uint32_t;
        auto getAttribute(uint32_t index) const -> VertexAttribute;

        auto getSize() const -> uint32_t;

    private:
        std::vector<VertexAttribute> attrs;
        uint32_t size = 0;
    };

    inline auto VertexBufferLayout::getSize() const -> uint32_t
    {
        return size;
    }

    inline auto VertexBufferLayout::getAttribute(uint32_t index) const -> VertexAttribute
    {
        return attrs.at(index);
    }

    inline auto VertexBufferLayout::getAttributeCount() const -> uint32_t
    {
        return static_cast<uint32_t>(attrs.size());
    }

    inline void VertexBufferLayout::addAttribute(uint32_t elementCount, uint32_t location)
    {
        auto size = static_cast<uint32_t>(sizeof(float) * elementCount);
        attrs.push_back(VertexAttribute{elementCount, size, location});
        this->size += size;
    }
}
