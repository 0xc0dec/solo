#pragma once

#include "SoloBase.h"

namespace solo
{
    struct VertexFormatElement
    {
        enum class Semantics
        {
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

        Semantics semantics;
        unsigned size;

        VertexFormatElement(Semantics semantics, unsigned size):
            semantics(semantics),
            size(size)
        {
        }
    };

    class VertexFormat
    {
    public:
        VertexFormat() {}
        VertexFormat(const std::vector<VertexFormatElement> &elements);

        size_t getElementCount() const;
        VertexFormatElement getElement(size_t index) const;

        unsigned getVertexSize() const;

    private:
        unsigned vertexSize = 0;
        std::vector<VertexFormatElement> elements;
    };

    inline size_t VertexFormat::getElementCount() const
    {
        return elements.size();
    }

    inline VertexFormatElement VertexFormat::getElement(size_t index) const
    {
        return elements[index];
    }

    inline unsigned VertexFormat::getVertexSize() const
    {
        return vertexSize;
    }
}
