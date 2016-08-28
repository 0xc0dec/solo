#pragma once

#include <vector>


namespace solo
{
    enum class VertexBufferLayoutSemantics
    {
        Position = 0,
        Normal,
        Color,
        Tangent,
        Binormal,
        TexCoord0,
        TexCoord1,
        TexCoord2,
        TexCoord3,
        TexCoord4,
        TexCoord5,
        TexCoord6,
        TexCoord7
    };

    struct VertexBufferLayoutElement final
    {
        VertexBufferLayoutSemantics semantics;
        uint32_t size;
    };

    class VertexBufferLayout final
    {
    public:
        VertexBufferLayout() {}
        ~VertexBufferLayout() {}

        void add(VertexBufferLayoutSemantics semantics, uint32_t size);

        auto getElementCount() const -> uint32_t;
        auto getElement(uint32_t index) const -> const VertexBufferLayoutElement&;

        auto getSize() const -> uint32_t;

    private:
        std::vector<VertexBufferLayoutElement> elements;
        uint32_t size = 0;
    };

    inline auto VertexBufferLayout::getSize() const -> uint32_t
    {
        return size;
    }

    inline auto VertexBufferLayout::getElement(uint32_t index) const -> const VertexBufferLayoutElement&
    {
        return elements[index];
    }

    inline auto VertexBufferLayout::getElementCount() const -> uint32_t
    {
        return static_cast<uint32_t>(elements.size());
    }

    inline void VertexBufferLayout::add(VertexBufferLayoutSemantics semantics, uint32_t size)
    {
        elements.push_back(VertexBufferLayoutElement{ semantics, size });
        this->size += sizeof(float) * size;
    }
}
