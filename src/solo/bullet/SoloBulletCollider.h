/*
    Copyright (c) Aleksey Fedotov
    MIT license
*/

#pragma once

#include "SoloCommon.h"
#include <btBulletCollisionCommon.h>

namespace solo
{
    class BulletCollider: public NoCopyAndMove
    {
    public:
		virtual ~BulletCollider() = default;

        virtual auto getShape() -> btCollisionShape* = 0;

    protected:
		BulletCollider() = default;
    };
}