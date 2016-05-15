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
        BulletRigidBody(const Node& node, const RigidBodyConstructionParameters& parameters);
        ~BulletRigidBody();

        virtual void setCollider(sptr<Collider> collider) override final;

        virtual void update() override final;

    private:
        float mass = 0;
        sptr<Collider> collider;
        Transform* transformCmp;
        btDiscreteDynamicsWorld* world;
        uptr<btMotionState> motionState;
        uptr<btRigidBody> body;
    };
}