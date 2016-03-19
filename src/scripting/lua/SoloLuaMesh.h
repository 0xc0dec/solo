#pragma once

#include "SoloVertexFormat.h"

namespace solo
{
    class Mesh;

    class LuaMesh
    {
    public:
        static uint32_t addVertexBuffer(Mesh* mesh, const VertexBufferLayout& layout, const std::vector<float>& data, uint32_t vertexCount);
        static uint32_t addDynamicVertexBuffer(Mesh* mesh, const VertexBufferLayout& layout, const std::vector<float>& data, uint32_t vertexCount);
        static void updateDynamicVertexBuffer(Mesh* mesh, uint32_t index, uint32_t vertexOffset, const std::vector<float>& data, uint32_t vertexCount);
        static uint32_t addPart(Mesh* mesh, const std::vector<uint16_t>& data, uint32_t elementCount);
    };
}