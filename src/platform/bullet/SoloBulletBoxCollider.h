#pragma once

#include "SoloBulletCollider.h"
#include "SoloBoxCollider.h"


namespace solo
{
    class Vector3;

    class BulletBoxCollider final: public BulletCollider, public BoxCollider
    {
    public:
        explicit BulletBoxCollider(const Vector3& halfExtents);

        auto getShape() -> btCollisionShape* override final;

    private:
        uptr<btBoxShape> shape;
    };

    inline auto BulletBoxCollider::getShape() -> btCollisionShape*
    {
        return shape.get();
    }
}
