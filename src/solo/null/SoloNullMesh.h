/*
    Copyright (c) Aleksey Fedotov
    MIT license
*/

#pragma once

#include "SoloMesh.h"

namespace solo
{
    namespace null
    {
        class Mesh final : public solo::Mesh
        {
        public:
            auto addVertexBuffer(const VertexBufferLayout &layout, const void *data, u32 vertexCount) -> u32 override final
            {
                return 0;
            }
            
            auto addDynamicVertexBuffer(const VertexBufferLayout &layout, const void *data, u32 vertexCount) -> u32 override final
            {
                return 0;
            }

            void updateDynamicVertexBuffer(u32 index, u32 vertexOffset, const void *data, u32 vertexCount) override final {}

            void removeVertexBuffer(u32 index) override final {}

            auto addPart(const void *indexData, u32 indexElementCount) -> u32 override final
            {
                return 0;
            }

            void removePart(u32 index) override final {}

            auto getPartCount() const -> u32 override
            {
                return 0;
            }

            auto getPrimitiveType() const -> PrimitiveType override final
            {
                return PrimitiveType::Lines;
            }

            void setPrimitiveType(PrimitiveType type) override final {}
        };
    }
}