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

    struct VertexBufferLayoutElement
    {
        VertexBufferLayoutSemantics semantics;
        uint32_t size;
    };

    class VertexBufferLayout
    {
    public:
        VertexBufferLayout() {}
        ~VertexBufferLayout() {}

        void add(VertexBufferLayoutSemantics semantics, uint32_t elementCount);

        uint32_t getElementCount() const;
        const VertexBufferLayoutElement& getElement(uint32_t index) const;

        uint32_t getSize() const;

    private:
        std::vector<VertexBufferLayoutElement> elements;
        uint32_t size = 0;
    };

    inline uint32_t VertexBufferLayout::getSize() const
    {
        return size;
    }

    inline const VertexBufferLayoutElement& VertexBufferLayout::getElement(uint32_t index) const
    {
        return elements[index];
    }

    inline uint32_t VertexBufferLayout::getElementCount() const
    {
        return static_cast<uint32_t>(elements.size());
    }

    inline void VertexBufferLayout::add(VertexBufferLayoutSemantics semantics, uint32_t elementCount)
    {
        elements.push_back(VertexBufferLayoutElement{ semantics, elementCount });
        this->size += sizeof(float) * elementCount;
    }
}
