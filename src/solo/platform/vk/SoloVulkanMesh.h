/*
    Copyright (c) Aleksey Fedotov
    MIT license
*/

#pragma once

#include "SoloCommon.h"

#ifdef SL_VULKAN_RENDERER

#include "SoloMesh.h"
#include "SoloVulkanBuffer.h"
#include "SoloVulkanPipeline.h"
#include <vector>

namespace solo
{
    namespace vk
    {
        class Renderer;

        class Mesh final: public solo::Mesh
        {
        public:
            explicit Mesh(Device *device);
            Mesh(Device *device, MeshData *data);
            ~Mesh();

            auto addVertexBuffer(const VertexBufferLayout &layout, const void *data, uint32_t vertexCount) -> uint32_t override final;
            auto addDynamicVertexBuffer(const VertexBufferLayout &layout, const void *data, uint32_t vertexCount) -> uint32_t override final;
            void updateDynamicVertexBuffer(uint32_t index, uint32_t vertexOffset, const void *data, uint32_t vertexCount) override final;
            void removeVertexBuffer(uint32_t index) override final;

            void buildPipeline(PipelineBuilder &builder) const;

            // TODO needed?
            auto getVertexBufferLayout(uint32_t index) const -> VertexBufferLayout;
            auto getVertexBuffer(uint32_t index) const -> VkBuffer;

            auto addPart(const void *indexData, uint32_t indexElementCount) -> uint32_t override final;
            void removePart(uint32_t index) override final;
            auto getPartCount() const -> uint32_t override final;

            void draw(Effect *effect) override final;
            void drawPart(Effect *effect, uint32_t part) override final;

            // TODO move these to base class?
            auto getPrimitiveType() const -> PrimitiveType override final;
            void setPrimitiveType(PrimitiveType type) override final;

        private:
            Renderer *renderer = nullptr;

            std::vector<Buffer> vertexBuffers;
            std::vector<VertexBufferLayout> layouts;
            std::vector<uint32_t> vertexCounts;
        };

        inline auto Mesh::getVertexBufferLayout(uint32_t index) const -> VertexBufferLayout
        {
            return layouts.at(index);
        }

        inline auto Mesh::getVertexBuffer(uint32_t index) const -> VkBuffer
        {
            return vertexBuffers.at(index).getHandle();
        }
    }
}

#endif