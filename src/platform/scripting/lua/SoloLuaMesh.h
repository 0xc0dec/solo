#pragma once

#include "SoloVertexFormat.h"

namespace solo
{
    class Mesh;
    class IndexedMeshPart;

    class LuaMesh
    {
    public:
        static IndexedMeshPart *addPart(Mesh *mesh);
        static void resetVertexData(Mesh *mesh, const std::vector<float> &data, unsigned elementCount, bool dynamic);
        static void updateVertexData(Mesh *mesh, const std::vector<float> &data, unsigned elementCount, unsigned updateFromIndex);
    };
}