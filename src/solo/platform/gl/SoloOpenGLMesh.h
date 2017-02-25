/*
    Copyright (c) Aleksey Fedotov
    MIT license
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
            ~Mesh();

            auto addVertexBuffer(const VertexBufferLayout &layout, const void *data, uint32_t vertexCount) -> uint32_t override final;
            auto addDynamicVertexBuffer(const VertexBufferLayout &layout, const void *data, uint32_t vertexCount) -> uint32_t override final;
            void updateDynamicVertexBuffer(uint32_t index, uint32_t vertexOffset, const void *data, uint32_t vertexCount) override final;
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
            bool dirtyEffectBinding = true;

            auto addVertexBuffer(uint32_t bufferHandle, const VertexBufferLayout &layout, uint32_t vertexCount) -> uint32_t;

            void rebuildEffectBinding(solo::Effect *effect);
            void updateMinVertexCount();
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
