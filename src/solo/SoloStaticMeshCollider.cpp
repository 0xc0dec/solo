/* 
 * Copyright (c) Aleksey Fedotov 
 * MIT license 
 */

#include "SoloStaticMeshCollider.h"
#include "SoloJobPool.h"
#include "bullet/SoloBulletStaticMeshCollider.h"

using namespace solo;

auto StaticMeshCollider::create(Device *device) -> sptr<StaticMeshCollider>
{
    return std::make_shared<BulletStaticMeshCollider>();
}

auto StaticMeshCollider::fromFileAsync(Device *device, const str &path) -> sptr<AsyncHandle<StaticMeshCollider>>
{
    auto handle = std::make_shared<AsyncHandle<StaticMeshCollider>>();

    // TODO

    return handle;
}
