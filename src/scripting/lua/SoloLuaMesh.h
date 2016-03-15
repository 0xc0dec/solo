#pragma once

#include "SoloVertexFormat.h"

namespace solo
{
    class Mesh;

    class LuaMesh
    {
    public:
        static void addBuffer(Mesh* mesh, const VertexBufferLayout& layout, const std::vector<float>& data, uint32_t elementCount);
        static void addPart(Mesh* mesh, const std::vector<float>& data, uint32_t elementCount);
    };
}