#pragma once

#include "SoloVertexFormat.h"

namespace solo
{
    class Mesh;

    class LuaMesh
    {
    public:
        static int addIndex(Mesh* mesh);
        static void resetStorage(Mesh* mesh, int storageId, const std::vector<float>& data, int elementCount, bool dynamic);
        static void updateStorage(Mesh* mesh, int storageId, const std::vector<float>& data, int elementCount, int updateFromIndex);
        static void resetIndexData(Mesh* mesh, int index, const std::vector<uint16_t>& data, int elementCount, bool dynamic);
        static void updateIndexData(Mesh* mesh, int index, const std::vector<uint16_t>& data, int elementCount, int updateFromIndex);
    };
}