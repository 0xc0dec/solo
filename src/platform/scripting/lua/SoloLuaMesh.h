#pragma once

#include "SoloVertexFormat.h"

namespace solo
{
    class Mesh;

    class LuaMesh
    {
    public:
        static void addBuffer(Mesh* mesh, const VertexBufferLayout& layout, const std::vector<float>& data, int elementCount);
        static void addIndex(Mesh* mesh, const std::vector<float>& data, int elementCount);
    };
}