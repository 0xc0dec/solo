#pragma once

#include "SoloRigidBody.h"
#include <btBulletDynamicsCommon.h>


namespace solo
{
    class Transform;
    class Collider;

    class BulletRigidBody final : public RigidBody
    {
    public:
        BulletRigidBody(const Node& node, const RigidBodyConstructionParameters& parameters);

    private:
        virtual void onComponentAdded(Component* cmp) override final;
        virtual void onComponentRemoved(Component* cmp) override final;

        void syncTransform();

        Collider* collider = nullptr;

        Transform* transformCmp;
        btDiscreteDynamicsWorld* world;
        btTransform transform;
        uptr<btMotionState> motionState;
        uptr<btRigidBody> body;
    };
}