/*
    Copyright (c) Aleksey Fedotov
    MIT license
*/

#include "SoloVulkanMesh.h"

#ifdef SL_VULKAN_RENDERER

#include "SoloDevice.h"
#include "SoloVulkanRenderer.h"

using namespace solo;


vk::Mesh::Mesh(Device *device)
{
    renderer = dynamic_cast<Renderer*>(device->getRenderer());
}


vk::Mesh::Mesh(Device *device, MeshData *data):
    Mesh(device)
{
}


vk::Mesh::~Mesh()
{
}


auto vk::Mesh::addVertexBuffer(const VertexBufferLayout &layout, const float *data, uint32_t vertexCount) -> uint32_t
{
    auto buf = Buffer(renderer->getDevice(), layout.getSize() * vertexCount,
        Buffer::Device | Buffer::Vertex | Buffer::TransferDst,
        renderer->getPhysicalDeviceMemProps());
    // TODO transfer data
    vertexBuffers.push_back(std::move(buf));
    return static_cast<uint32_t>(vertexBuffers.size() - 1);
}


auto vk::Mesh::addDynamicVertexBuffer(const VertexBufferLayout &layout, const float *data, uint32_t vertexCount) -> uint32_t
{
    return 0;
}


void vk::Mesh::updateDynamicVertexBuffer(uint32_t index, uint32_t vertexOffset, const float *data, uint32_t vertexCount)
{
}


void vk::Mesh::removeVertexBuffer(uint32_t index)
{
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


void vk::Mesh::draw(Effect *effect)
{
}


void vk::Mesh::drawPart(Effect *effect, uint32_t part)
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
