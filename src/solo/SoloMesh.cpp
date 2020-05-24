/* 
 * Copyright (c) Aleksey Fedotov 
 * MIT license 
 */

#include "SoloMesh.h"
#include "SoloEffect.h"
#include "SoloDevice.h"
#include "SoloJobPool.h"
#include "SoloMeshData.h"
#include "gl/SoloOpenGLMesh.h"
#include "vk/SoloVulkanMesh.h"

using namespace solo;

auto Mesh::empty(Device *device) -> sptr<Mesh>
{
    switch (device->mode())
    {
#ifdef SL_OPENGL_RENDERER
    case DeviceMode::OpenGL:
        return std::make_shared<OpenGLMesh>();
#endif
#ifdef SL_VULKAN_RENDERER
    case DeviceMode::Vulkan:
        return std::make_shared<VulkanMesh>(device);
#endif
    default:
        SL_DEBUG_PANIC(true, "Unknown device mode");
        return nullptr;
    }
}

static auto fromData(Device *device, sptr<MeshData> data, const VertexBufferLayout &bufferLayout) -> sptr<Mesh>
{
    auto mesh = Mesh::empty(device);

    mesh->addVertexBuffer(bufferLayout, data->vertexData(), data->vertexData().size() / bufferLayout.elementCount());

	for (auto part = 0; part < data->partsCount(); part++)
		mesh->addPart(data->indexData(part), data->indexElementCount(part));

    return mesh;
}

auto Mesh::fromFile(Device *device, const str &path, const VertexBufferLayout &bufferLayout) -> sptr<Mesh>
{
    const auto data = MeshData::fromFile(device, path, bufferLayout);
    return fromData(device, data, bufferLayout);
}

auto Mesh::fromFileAsync(Device *device, const str &path, const VertexBufferLayout &bufferLayout)
    -> sptr<AsyncHandle<Mesh>>
{
    auto handle = std::make_shared<AsyncHandle<Mesh>>();

    MeshData::fromFileAsync(device, path, bufferLayout)->done(
        [handle, device, bufferLayout](sptr<MeshData> data)
        {
            handle->resolve(fromData(device, data, bufferLayout));
        });

    return handle;
}

void Mesh::updateMinVertexCount()
{
    constexpr auto max = (std::numeric_limits<u32>::max)();

    minVertexCount_ = max;

    for (const auto &count : vertexCounts_)
        minVertexCount_ = (std::min)(count, minVertexCount_);

    if (minVertexCount_ == max)
        minVertexCount_ = 0;
}

auto Mesh::addVertexBuffer(const VertexBufferLayout &layout, const vec<float> &data, u32 vertexCount) -> u32
{
	layouts_.push_back(layout);
    vertexCounts_.push_back(vertexCount);
	vertexData_.push_back(data); // TODO move
	updateMinVertexCount();
	return static_cast<u32>(vertexCounts_.size() - 1);
}

auto Mesh::addDynamicVertexBuffer(const VertexBufferLayout &layout, const vec<float> &data, u32 vertexCount) -> u32
{
	// TODO No copy-paste
	layouts_.push_back(layout);
    vertexCounts_.push_back(vertexCount);
	vertexData_.push_back(data); // TODO move
	updateMinVertexCount();
	return static_cast<u32>(vertexCounts_.size() - 1);
}

void Mesh::updateDynamicVertexBuffer(u32 index, u32 vertexOffset, const void *data, u32 vertexCount)
{
	// TODO
}

void Mesh::removeVertexBuffer(u32 index)
{
    vertexCounts_.erase(vertexCounts_.begin() + index);
	vertexData_.erase(vertexData_.begin() + index);
    layouts_.erase(layouts_.begin() + index);
	updateMinVertexCount();
}

auto Mesh::addPart(const vec<u32> &data, u32 elementCount) -> u32
{
	indexElementCounts_.push_back(elementCount);
	partData_.push_back(data);
    return static_cast<u32>(indexElementCounts_.size() - 1);
}

void Mesh::removePart(u32 part)
{
	indexElementCounts_.erase(indexElementCounts_.begin() + part);
	partData_.erase(partData_.begin() + part);
}
