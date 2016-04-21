#pragma once

#include "SoloBoxCollider.h"
#include <btBulletCollisionCommon.h>


namespace solo
{
    class BulletBoxCollider : public BoxCollider
    {
    public:
        
    private:
        friend class BoxCollider;

        BulletBoxCollider(const Node& node, const Vector3& halfExtents);

        uptr<btBoxShape> shape;
    };
}
