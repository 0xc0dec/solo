/*
    Copyright (c) Aleksey Fedotov
    MIT license
*/

#pragma once

#include "SoloCommon.h"
#include <btBulletCollisionCommon.h>

namespace solo
{
    namespace bullet
    {
        class Collider
        {
        public:
            SL_DISABLE_COPY_AND_MOVE(Collider)

            virtual ~Collider() {}

            virtual auto getShape() -> btCollisionShape* = 0;

        protected:
            Collider() {}
        };
    }
}