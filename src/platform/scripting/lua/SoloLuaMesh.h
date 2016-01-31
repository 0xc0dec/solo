#pragma once

#include "SoloVertexFormat.h"

namespace solo
{
    class Mesh;

    class LuaMesh
    {
    public:
        static int addIndex(Mesh* mesh);
        // TODO other index methods
        static void resetData(Mesh* mesh, int slot, const std::vector<float>& data, int elementCount, bool dynamic);
        static void updateData(Mesh* mesh, int slot, const std::vector<float>& data, int elementCount, int updateFromIndex);
        static void resetIndexData(Mesh* mesh, int index, const std::vector<uint16_t>& data, int elementCount, bool dynamic);
        static void updateIndexData(Mesh* mesh, int index, const std::vector<uint16_t>& data, int elementCount, int updateFromIndex);
    };
}