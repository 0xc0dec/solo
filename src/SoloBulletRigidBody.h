#pragma once

#include "SoloRigidBody.h"
#include <btBulletDynamicsCommon.h>


namespace solo
{
    class Transform;

    class BulletRigidBody final : public RigidBody
    {
    public:

    private:
        friend class RigidBody;

        BulletRigidBody(const Node& node, const RigidBodyConstructionParameters& parameters);

        virtual void onComponentAdded(Component* cmp) override final;
        virtual void onComponentRemoved(Component* cmp) override final;

        void syncTransform();

        Transform* transformCmp;
        btDiscreteDynamicsWorld* world;
        btTransform transform;
        uptr<btMotionState> motionState;
        uptr<btRigidBody> body;
    };
}