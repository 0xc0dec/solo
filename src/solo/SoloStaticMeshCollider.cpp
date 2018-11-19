/* 
 * Copyright (c) Aleksey Fedotov 
 * MIT license 
 */

#include "SoloStaticMeshCollider.h"
#include "SoloJobPool.h"
#include "SoloMeshData.h"
#include "SoloVertexBufferLayout.h"
#include "bullet/SoloBulletStaticMeshCollider.h"

using namespace solo;

auto StaticMeshCollider::fromFile(Device *device, const str &path) -> sptr<StaticMeshCollider>
{
    VertexBufferLayout layout;
    layout.addAttribute(VertexAttributeSemantics::Position);
    const auto data = MeshData::fromFile(device, path, layout);
    return std::make_shared<BulletStaticMeshCollider>(data);
}

auto StaticMeshCollider::fromFileAsync(Device *device, const str &path) -> sptr<AsyncHandle<StaticMeshCollider>>
{
    auto handle = std::make_shared<AsyncHandle<StaticMeshCollider>>();

    VertexBufferLayout layout;
    layout.addAttribute(VertexAttributeSemantics::Position);
    MeshData::fromFileAsync(device, path, layout)->done(
        [handle](sptr<MeshData> data)
        {
            handle->resolve(std::make_shared<BulletStaticMeshCollider>(data));
        });

    return handle;
}
