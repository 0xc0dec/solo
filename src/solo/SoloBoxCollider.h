/*
 * Copyright (c) Aleksey Fedotov
 * MIT license
 */

#pragma once

#include "SoloCollider.h"
#include "SoloCommon.h"

namespace solo {
    class Vector3;

    class BoxCollider: public Collider {
    public:
        static auto create(const Vector3 &size) -> sptr<BoxCollider>;

    protected:
        BoxCollider() = default;
    };
}