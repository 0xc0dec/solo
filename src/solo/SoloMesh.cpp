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

    mesh->addVertexBuffer(bufferLayout, data->vertexData().data(),
        data->vertexData().size() / bufferLayout.elementCount());

	for (auto part = 0; part < data->partsCount(); part++)
		mesh->addPart(data->indexData(part), data->indexElementCount(part), data->indexElementSize());

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
