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
    
    namespace bullet
    {
        class Collider;

        class RigidBody final : public solo::RigidBody
        {
        public:
            RigidBody(const Node &node, const RigidBodyConstructionParameters &parameters);
            virtual ~RigidBody();

            void setCollider(sptr<solo::Collider> collider) override final;

            void onTransformChanged(const Transform *transform) override final;

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
}