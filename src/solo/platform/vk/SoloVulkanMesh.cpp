/*
    Copyright (c) Aleksey Fedotov
    MIT license
*/

#include "SoloVulkanMesh.h"

#ifdef SL_VULKAN_RENDERER

#include "SoloDevice.h"
#include "SoloVulkanRenderer.h"
#include "SoloVulkanRenderCommand.h"

using namespace solo;


vk::Mesh::Mesh(Device *device)
{
    renderer = dynamic_cast<Renderer*>(device->getRenderer());
}


vk::Mesh::~Mesh()
{
}


auto vk::Mesh::addVertexBuffer(const VertexBufferLayout &layout, const void *data, uint32_t vertexCount) -> uint32_t
{
    auto stagingBuf = Buffer(renderer->getDevice(), layout.getSize() * vertexCount,
        Buffer::Host | Buffer::TransferSrc, renderer->getPhysicalDeviceMemProps());
    stagingBuf.update(data);

    auto buf = Buffer(renderer->getDevice(), layout.getSize() * vertexCount,
        Buffer::Device | Buffer::Vertex | Buffer::TransferDst,
        renderer->getPhysicalDeviceMemProps());
    stagingBuf.transferTo(buf, renderer->getQueue(), renderer->getCommandPool());

    vertexBuffers.push_back(std::move(buf));
    layouts.push_back(layout);
    vertexCounts.push_back(vertexCount);

    return static_cast<uint32_t>(vertexBuffers.size() - 1);
}


auto vk::Mesh::addDynamicVertexBuffer(const VertexBufferLayout &layout, const void *data, uint32_t vertexCount) -> uint32_t
{
    return 0;
}


void vk::Mesh::updateDynamicVertexBuffer(uint32_t index, uint32_t vertexOffset, const void *data, uint32_t vertexCount)
{
}


void vk::Mesh::removeVertexBuffer(uint32_t index)
{
    vertexBuffers.erase(vertexBuffers.begin() + index);
    layouts.erase(layouts.begin() + index);
    vertexCounts.erase(vertexCounts.begin() + index);
}


auto vk::Mesh::addPart(const void *indexData, uint32_t indexElementCount) -> uint32_t
{
    auto size = sizeof(uint16_t) * indexElementCount;

    auto stagingBuffer = Buffer(renderer->getDevice(), size, Buffer::Host | Buffer::TransferSrc, renderer->getPhysicalDeviceMemProps());
    stagingBuffer.update(indexData);

    auto buf = Buffer(renderer->getDevice(), size, Buffer::Device | Buffer::Index | Buffer::TransferDst, renderer->getPhysicalDeviceMemProps());
    stagingBuffer.transferTo(buf, renderer->getQueue(), renderer->getCommandPool());

    indexBuffers.push_back(std::move(buf));
    indexElementCounts.push_back(indexElementCount);

    return static_cast<uint32_t>(indexElementCounts.size() - 1);
}


void vk::Mesh::removePart(uint32_t index)
{
    indexBuffers.erase(indexBuffers.begin() + index);
    indexElementCounts.erase(indexElementCounts.begin() + index);
}


auto vk::Mesh::getPartCount() const -> uint32_t
{
    return indexBuffers.size();
}


void vk::Mesh::draw()
{
    renderer->addRenderCommand(RenderCommand::drawMesh(this));
}


void vk::Mesh::drawPart(uint32_t part)
{
    renderer->addRenderCommand(RenderCommand::drawMesh(this));
}


auto vk::Mesh::getPrimitiveType() const -> PrimitiveType
{
    return PrimitiveType::Triangles;
}


void vk::Mesh::setPrimitiveType(PrimitiveType type)
{
}


#endif
