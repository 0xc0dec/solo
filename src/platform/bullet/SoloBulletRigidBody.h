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
        virtual ~BulletRigidBody();

        void setCollider(sptr<Collider> collider) override final;

        void onTransformChanged(const Transform* transform, uint32_t dirtyFlags) override final;

    private:
        void syncScale();

        float mass = 0;
        sptr<Collider> collider;
        btCollisionShape* shape;
        Transform* transformCmp;
        btDiscreteDynamicsWorld* world;
        uptr<btMotionState> motionState;
        uptr<btRigidBody> body;
    };
}