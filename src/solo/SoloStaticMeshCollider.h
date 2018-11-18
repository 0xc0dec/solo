/* 
 * Copyright (c) Aleksey Fedotov 
 * MIT license 
 */

#pragma once

#include "SoloCollider.h"
#include "SoloAsyncHandle.h"

namespace solo
{
    class Device;

    class StaticMeshCollider: public Collider
    {
    public:
        static auto create(Device *device) -> sptr<StaticMeshCollider>;
        static auto fromFileAsync(Device *device, const str &path) -> sptr<AsyncHandle<StaticMeshCollider>>;

    protected:
        StaticMeshCollider() = default;
    };
}