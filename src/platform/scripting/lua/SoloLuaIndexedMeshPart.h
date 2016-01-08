#pragma once

#include "SoloIndexedMeshPart.h"

namespace solo
{
    class IndexedMeshPart;

    class LuaIndexedMeshPart
    {
    public:
        static void resetIndexData(IndexedMeshPart *part, const std::vector<uint16_t> &data, unsigned elementCount, bool dynamic);
        static void updateIndexData(IndexedMeshPart *part, const std::vector<uint16_t> &data, unsigned elementCount, unsigned updateFromIndex);
    };
}