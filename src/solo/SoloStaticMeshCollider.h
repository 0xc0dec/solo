/* 
 * Copyright (c) Aleksey Fedotov 
 * MIT license 
 */

#pragma once

#include "SoloCollider.h"

namespace solo
{
    class StaticMeshCollider: public Collider
    {
    public:
        static auto create() -> sptr<StaticMeshCollider>;

    protected:
        StaticMeshCollider() = default;
    };
}