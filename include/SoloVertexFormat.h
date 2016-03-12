#pragma once

#include "SoloBase.h"
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
        SL_DEBUG_FMT_THROW_IF(index < 0 || index >= elements.size(), InvalidInputException, "Invalid index")
        return elements[index];
    }

    inline int VertexBufferLayout::getElementCount() const
    {
        return static_cast<int>(elements.size());
    }

    inline void VertexBufferLayout::add(VertexBufferLayoutSemantics semantics, int elementCount)
    {
        SL_DEBUG_FMT_THROW_IF(elementCount <= 0, InvalidInputException, "Negative element count")
        elements.push_back(VertexBufferLayoutElement{ semantics, elementCount });
        this->size += sizeof(float) * elementCount;
    }
}
