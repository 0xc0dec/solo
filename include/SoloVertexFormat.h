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
        int storageId;

        VertexFormatElement(VertexFormatElementSemantics semantics, int size, int storageId):
            semantics(semantics),
            size(size),
            storageId(storageId)
        {
        }
    };

    class VertexFormat
    {
    public:
        VertexFormat() {}
        VertexFormat(const std::vector<VertexFormatElement>& elements);

        int getElementCount() const;
        VertexFormatElement getElement(int index) const;

        int getVertexSize(int perStorageId) const;
        int getStorageCount() const;

    private:
        std::unordered_map<int, int> sizesPerStorage;
        std::vector<VertexFormatElement> elements;
        int storageCount = 0;
    };

    inline int VertexFormat::getElementCount() const
    {
        return static_cast<int>(elements.size());
    }

    inline VertexFormatElement VertexFormat::getElement(int index) const
    {
        return elements[index];
    }

    inline int VertexFormat::getVertexSize(int perStorageId) const
    {
        return sizesPerStorage.at(perStorageId);
    }

    inline int VertexFormat::getStorageCount() const
    {
        return static_cast<int>(sizesPerStorage.size());
    }
}
