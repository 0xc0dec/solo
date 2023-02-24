/*
 * Copyright (c) Aleksey Fedotov
 * MIT license
 */

#pragma once

#include "SoloCollider.h"
#include "SoloAsyncHandle.h"

namespace solo {
    class Device;
    class Mesh;

    class StaticMeshCollider: public Collider {
    public:
        static auto fromMesh(sptr<Mesh> mesh) -> sptr<StaticMeshCollider>;

    protected:
        StaticMeshCollider() = default;
    };
}