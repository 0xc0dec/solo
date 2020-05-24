/* 
 * Copyright (c) Aleksey Fedotov 
 * MIT license 
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
        BulletRigidBody(const Node &node, const RigidBodyParams &params);
        virtual ~BulletRigidBody();

        void update() override;

        void setCollider(sptr<Collider> collider) override;

        bool isKinematic() override;
        void setKinematic(bool kinematic) override;

        bool isStatic() override;
        void setStatic(bool isStatic) override;
    	
    private:
        float mass_ = 0;
        sptr<Collider> collider_;
        btCollisionShape *shape_;
        Transform *transformCmp_;
        btDiscreteDynamicsWorld *world_;
        uptr<btMotionState> motionState_;
        uptr<btRigidBody> body_;
        u32 lastTransformVersion_ = ~0;

        void syncScale() const;
    	void syncPosition() const;
    };
}