/*
    Copyright (c) Aleksey Fedotov

    This software is provided 'as-is', without any express or implied
    warranty. In no event will the authors be held liable for any damages
    arising from the use of this software.

    Permission is granted to anyone to use this software for any purpose,
    including commercial applications, and to alter it and redistribute it
    freely, subject to the following restrictions:

    1. The origin of this software must not be misrepresented; you must not
        claim that you wrote the original software. If you use this software
        in a product, an acknowledgment in the product documentation would be
        appreciated but is not required.
    2. Altered source versions must be plainly marked as such, and must not be
        misrepresented as being the original software.
    3. This notice may not be removed or altered from any source distribution.
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
            auto addVertexBuffer(const VertexBufferLayout &layout, const float *data, uint32_t vertexCount) -> uint32_t override final
            {
                return 0;
            }

            auto addDynamicVertexBuffer(const VertexBufferLayout &layout, const float *data, uint32_t vertexCount) -> uint32_t override final
            {
                return 0;
            }

            void updateDynamicVertexBuffer(uint32_t index, uint32_t vertexOffset, const float *data, uint32_t vertexCount) override final {}

            void removeVertexBuffer(uint32_t index) override final {}

            auto addPart(const void *indexData, uint32_t indexElementCount) -> uint32_t override final
            {
                return 0;
            }

            void removePart(uint32_t index) override final {}

            auto getPartCount() const -> uint32_t override
            {
                return 0;
            }

            void draw(Effect *effect) override final {}
            void drawPart(Effect *effect, uint32_t part) override final {}

            auto getPrimitiveType() const -> PrimitiveType override final
            {
                return PrimitiveType::Lines;
            }

            void setPrimitiveType(PrimitiveType type) override final {}
        };
    }
}