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
}


void vk::Mesh::buildPipeline(PipelineBuilder &builder) const
{
    auto layout = layouts[0];
    builder.withVertexSize(layout.getSize());
    uint32_t offset = 0;
    for (size_t i = 0; i < layout.getAttributeCount(); ++i)
    {
        builder = builder.withVertexAttribute(i, 0, VK_FORMAT_R32G32_SFLOAT, offset);
        offset += layout.getAttribute(i).size * sizeof(float); // TODO shouldn't do sizeof(float) itself
    }
}


auto vk::Mesh::addPart(const void *indexData, uint32_t indexElementCount) -> uint32_t
{
    return 0;
}


void vk::Mesh::removePart(uint32_t index)
{
}


auto vk::Mesh::getPartCount() const -> uint32_t
{
    return 0;
}


void vk::Mesh::draw()
{
    renderer->addRenderCommand(RenderCommand::drawMesh(this));
}


void vk::Mesh::drawPart(uint32_t part)
{
}


auto vk::Mesh::getPrimitiveType() const -> PrimitiveType
{
    return PrimitiveType::Triangles;
}


void vk::Mesh::setPrimitiveType(PrimitiveType type)
{
}


#endif
