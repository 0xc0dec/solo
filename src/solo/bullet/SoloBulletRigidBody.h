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

        void update() override final;

        void setCollider(sptr<Collider> collider) override final;

        bool isKinematic() override final;
        void setKinematic(bool kinematic) override final;

    private:
        float mass = 0;
        sptr<Collider> collider;
        btCollisionShape *shape;
        Transform *transformCmp;
        btDiscreteDynamicsWorld *world;
        uptr<btMotionState> motionState;
        uptr<btRigidBody> body;
        u32 lastTransformVersion = ~0;

        void syncScale();
    };
}