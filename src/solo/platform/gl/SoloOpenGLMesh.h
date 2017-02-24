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

#include "SoloCommon.h"

#ifdef SL_OPENGL_RENDERER

#include "SoloMesh.h"
#include "SoloOpenGLRenderer.h"

namespace solo
{
    class Effect;

    namespace gl
    {
        class Effect;

        class Mesh final : public solo::Mesh
        {
        public:
            explicit Mesh(Device *device);
            Mesh(Device *device, MeshData *data);
            Mesh(Device *device, MeshPrefab prefab);
            ~Mesh();

            auto addVertexBuffer(const VertexBufferLayout &layout, const float *data, uint32_t vertexCount) -> uint32_t override final;
            auto addDynamicVertexBuffer(const VertexBufferLayout &layout, const float *data, uint32_t vertexCount) -> uint32_t override final;
            void updateDynamicVertexBuffer(uint32_t index, uint32_t vertexOffset, const float *data, uint32_t vertexCount) override final;
            void removeVertexBuffer(uint32_t index) override final;

            auto addPart(const void *indexData, uint32_t indexElementCount) -> uint32_t override final;
            void removePart(uint32_t index) override final;
            auto getPartCount() const -> uint32_t override final;

            void draw(solo::Effect *effect) override final;
            void drawPart(solo::Effect *effect, uint32_t part) override final;

            auto getPrimitiveType() const -> PrimitiveType override final;
            void setPrimitiveType(PrimitiveType type) override final;

        private:
            Renderer *renderer = nullptr;
            Effect *lastEffect = nullptr;

            PrimitiveType primitiveType = PrimitiveType::Triangles;
            std::vector<uint32_t> vertexBuffers;
            std::vector<uint32_t> indexBuffers;
            std::vector<uint32_t> vertexCounts;
            std::vector<uint32_t> vertexSizes;
            uint32_t minVertexCount = 0;
            uint32_t programBinding = EmptyHandle;

            auto addVertexBuffer(uint32_t bufferHandle, const VertexBufferLayout &layout, uint32_t vertexCount) -> uint32_t;

            void rebuildEffectBinding(solo::Effect *effect);
            void recalculateMinVertexCount();
        };

        inline void Mesh::setPrimitiveType(PrimitiveType type)
        {
            primitiveType = type;
        }

        inline auto Mesh::getPrimitiveType() const -> PrimitiveType
        {
            return primitiveType;
        }

        inline auto Mesh::getPartCount() const -> uint32_t
        {
            return static_cast<uint32_t>(indexBuffers.size());
        }
    }
}

#endif
