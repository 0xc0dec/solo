/*
    Copyright (c) Aleksey Fedotov
    MIT license
*/

#pragma once

#include "SoloCommon.h"
#include <btBulletCollisionCommon.h>

namespace solo
{
    class BulletCollider
    {
    public:
        SL_DISABLE_COPY_AND_MOVE(BulletCollider)

        virtual ~BulletCollider() {}

        virtual auto getShape() -> btCollisionShape* = 0;

    protected:
        BulletCollider() {}
    };
}