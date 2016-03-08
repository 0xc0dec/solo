#pragma once

#include "SoloBase.h"


namespace solo
{
    enum class VertexBufferLayoutSemantics
    {
        Unknown = 0,
        Position = 1,
        Normal = 2,
        Color = 3,
        Tangent = 4,
        Binormal = 5,
        TexCoord0 = 6,
        TexCoord1 = 7,
        TexCoord2 = 8,
        TexCoord3 = 9,
        TexCoord4 = 10,
        TexCoord5 = 11,
        TexCoord6 = 12,
        TexCoord7 = 13
    };

    struct VertexBufferLayoutElement
    {
        VertexBufferLayoutSemantics semantics;
        int size;
    };

    class VertexBufferLayout
    {
    public:
        VertexBufferLayout() {}
        ~VertexBufferLayout() {}

        void add(VertexBufferLayoutSemantics semantics, int elementCount);

        int getElementCount() const;
        const VertexBufferLayoutElement& getElement(int index) const;

        int getSize() const;

    private:
        std::vector<VertexBufferLayoutElement> elements;
        int size = 0;
    };

    inline int VertexBufferLayout::getSize() const
    {
        return size;
    }

    inline const VertexBufferLayoutElement& VertexBufferLayout::getElement(int index) const
    {
        SL_DEBUG_THROW_IF(index < 0 || index >= elements.size(), InvalidInputException, "Invalid index")
        return elements[index];
    }

    inline int VertexBufferLayout::getElementCount() const
    {
        return elements.size();
    }

    inline void VertexBufferLayout::add(VertexBufferLayoutSemantics semantics, int elementCount)
    {
        SL_DEBUG_THROW_IF(elementCount <= 0, InvalidInputException, "Negative element count")
        elements.push_back(VertexBufferLayoutElement{ semantics, elementCount });
        this->size += sizeof(float) * elementCount;
    }
}
