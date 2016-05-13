#pragma once

#include "SoloBoxCollider.h"
#include <btBulletCollisionCommon.h>


namespace solo
{
    class BulletBoxCollider : public BoxCollider
    {
    public:
        BulletBoxCollider(const Node& node, const Vector3& halfExtents);

    private:
        uptr<btBoxShape> shape;
    };
}
