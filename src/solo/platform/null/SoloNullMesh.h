#pragma once

#include "SoloMesh.h"


namespace solo
{
    class NullMesh: public Mesh
    {
    public:
        auto addVertexBuffer(const VertexBufferLayout& layout, const float* data, uint32_t vertexCount) -> uint32_t override final { return 0; }
        auto addDynamicVertexBuffer(const VertexBufferLayout& layout, const float* data, uint32_t vertexCount) -> uint32_t override final { return 0; }
        void updateDynamicVertexBuffer(uint32_t index, uint32_t vertexOffset, const float* data, uint32_t vertexCount) override final {}
        void removeVertexBuffer(uint32_t index) override final {}
        auto addPart(const void* indexData, uint32_t indexElementCount) -> uint32_t override final { return 0; }
        void removePart(uint32_t index) override final {}
        auto getPartCount() const -> uint32_t override { return 0; }
        void draw(Effect* effect) override final {}
        void drawPart(Effect* effect, uint32_t part) override final {}
        auto getPrimitiveType() const -> PrimitiveType override final { return PrimitiveType::Lines; }
        void setPrimitiveType(PrimitiveType type) override final {}
    };
}