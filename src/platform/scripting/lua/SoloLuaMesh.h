#pragma once

#include "SoloVertexFormat.h"

namespace solo
{
    class Mesh;

    class LuaMesh
    {
    public:
        static unsigned addIndex(Mesh *mesh);
        static void resetStorage(Mesh *mesh, unsigned storageId, const std::vector<float> &data, unsigned elementCount, bool dynamic);
        static void updateStorage(Mesh *mesh, unsigned storageId, const std::vector<float> &data, unsigned elementCount, unsigned updateFromIndex);
        static void resetIndexData(Mesh *mesh, unsigned index, const std::vector<uint16_t> &data, unsigned elementCount, bool dynamic);
        static void updateIndexData(Mesh *mesh, unsigned index, const std::vector<uint16_t> &data, unsigned elementCount, unsigned updateFromIndex);
    };
}