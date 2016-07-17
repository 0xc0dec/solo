#pragma once

#include "SoloCommon.h"
#include <btBulletCollisionCommon.h>


namespace solo
{
    class BulletCollider
    {
    public:
        virtual ~BulletCollider() {}
        SL_NONCOPYABLE(BulletCollider)

        virtual auto getShape() -> btCollisionShape* = 0;

    protected:
        BulletCollider() {}
    };
}