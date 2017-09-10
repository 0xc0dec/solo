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
            ~Mesh();

            auto addVertexBuffer(const VertexBufferLayout &layout, const void *data, uint32_t vertexCount) -> uint32_t override final;
            auto addDynamicVertexBuffer(const VertexBufferLayout &layout, const void *data, uint32_t vertexCount) -> uint32_t override final;
            void updateDynamicVertexBuffer(uint32_t index, uint32_t vertexOffset, const void *data, uint32_t vertexCount) override final;
            void removeVertexBuffer(uint32_t index) override final;

            auto addPart(const void *indexData, uint32_t indexElementCount) -> uint32_t override final;
            void removePart(uint32_t index) override final;
            auto getPartCount() const -> uint32_t override final;

            auto getPrimitiveType() const -> PrimitiveType override final;
            void setPrimitiveType(PrimitiveType type) override final;

            auto getVertexBufferCount() const -> uint32_t { return vertexBuffers.size(); }
            auto getVertexBufferLayout(uint32_t index) const -> VertexBufferLayout { return layouts.at(index); }
            auto getVertexBuffer(uint32_t index) const -> VkBuffer { return vertexBuffers.at(index).getHandle(); }
            auto getPartBuffer(uint32_t index) const -> VkBuffer { return indexBuffers.at(index).getHandle(); }
            auto getPartIndexElementCount(uint32_t index) const -> uint32_t { return indexElementCounts.at(index); }
            auto getMinVertexCount() const -> uint32_t { return minVertexCount; }

        private:
            Renderer *renderer = nullptr;

            std::vector<Buffer> vertexBuffers;
            std::vector<Buffer> indexBuffers;
            std::vector<VertexBufferLayout> layouts;
            std::vector<uint32_t> vertexCounts;
            std::vector<uint32_t> indexElementCounts;
            uint32_t minVertexCount = 0;

            void updateMinVertexCount();
        };
    }
}

#endif