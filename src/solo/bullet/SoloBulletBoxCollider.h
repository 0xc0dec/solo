/*
    Copyright (c) Aleksey Fedotov
    MIT license
*/

#pragma once

#include "SoloBulletCollider.h"
#include "SoloBoxCollider.h"

namespace solo
{
    class Vector3;

    namespace bullet
    {
        class BoxCollider final : public Collider, public solo::BoxCollider
        {
        public:
            explicit BoxCollider(const Vector3 &halfExtents);

            auto getShape() -> btCollisionShape* override final;

        private:
            uptr<btBoxShape> shape;
        };

        inline auto BoxCollider::getShape() -> btCollisionShape *
        {
            return shape.get();
        }
    }
}
