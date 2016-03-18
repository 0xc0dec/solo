#pragma once

#include "SoloVertexFormat.h"

namespace solo
{
    class Mesh;

    class LuaMesh
    {
    public:
        static uint32_t addBuffer(Mesh* mesh, const VertexBufferLayout& layout, const std::vector<float>& data, uint32_t vertexCount);
        static uint32_t addDynamicBuffer(Mesh* mesh, const VertexBufferLayout& layout, const std::vector<float>& data, uint32_t vertexCount);
        static void updateDynamicBuffer(Mesh* mesh, uint32_t index, uint32_t offset, const std::vector<float>& data, uint32_t vertexCount);
        static uint32_t addPart(Mesh* mesh, const std::vector<float>& data, uint32_t elementCount);
    };
}