/*
    Copyright (c) Aleksey Fedotov
    MIT license
*/

#pragma once

#include "SoloRigidBody.h"
#include <btBulletDynamicsCommon.h>

namespace solo
{
    class Transform;
    class Collider;
    class BulletCollider;

    class BulletRigidBody final : public RigidBody
    {
    public:
        BulletRigidBody(const Node &node, const RigidBodyConstructionParameters &parameters);
        virtual ~BulletRigidBody();

        void setCollider(sptr<solo::Collider> collider) override final;

        void handleTransformChanged(const Transform *transform) override final;

    private:
        float mass = 0;
        sptr<solo::Collider> collider;
        btCollisionShape *shape;
        Transform *transformCmp;
        btDiscreteDynamicsWorld *world;
        uptr<btMotionState> motionState;
        uptr<btRigidBody> body;

        void syncScale();
    };
}