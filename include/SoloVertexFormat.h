#pragma once

#include "SoloBase.h"


namespace solo
{
    enum class VertexFormatElementSemantics
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

    struct VertexFormatElement
    {
        VertexFormatElementSemantics semantics;
        int size;
        int slot;

        VertexFormatElement(VertexFormatElementSemantics semantics, int size, int slot):
            semantics(semantics),
            size(size),
            slot(slot)
        {
        }
    };

    class VertexFormat final
    {
    public:
        VertexFormat() {}
        VertexFormat(const std::vector<VertexFormatElement>& elements);

        int getElementCount() const;
        VertexFormatElement getElement(int index) const;

        int getVertexSize(int slot) const;
        int getSlotCount() const;

    private:
        std::unordered_map<int, int> sizesPerSlot;
        std::vector<VertexFormatElement> elements;
        int slotCount = 0;
    };

    inline int VertexFormat::getElementCount() const
    {
        return static_cast<int>(elements.size());
    }

    inline VertexFormatElement VertexFormat::getElement(int index) const
    {
        return elements[index];
    }

    inline int VertexFormat::getVertexSize(int slot) const
    {
        return sizesPerSlot.at(slot);
    }

    inline int VertexFormat::getSlotCount() const
    {
        return static_cast<int>(sizesPerSlot.size());
    }
}
