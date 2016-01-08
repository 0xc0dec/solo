#pragma once

#include "SoloVertexFormat.h"

namespace solo
{
    class Mesh;

    class LuaMesh
    {
    public:
        static void resetVertexData(Mesh *mesh, const VertexFormat &format, const std::vector<float> &data, unsigned elementCount, bool dynamic);
        static void updateVertexData(Mesh *mesh, const std::vector<float> &data, unsigned elementCount, unsigned updateFromIndex);
    };
}