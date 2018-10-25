/*
    Copyright (c) Aleksey Fedotov
    MIT license
*/

#pragma once

#include "SoloPhysics.h"
#include <btBulletDynamicsCommon.h>

namespace solo
{
    class BulletPhysics final : public Physics
    {
    public:
        BulletPhysics(Device *device, const FriendToken<Device> &deviceToken);

        void update() override final;

        void setGravity(const Vector3 &gravity) override final;

        auto rayTestFirst(const Vector3 &from, const Vector3 &to) -> RayTestResult override final;
        auto rayTestAll(const Vector3 &from, const Vector3 &to) -> vec<RayTestResult> override final;

        auto world() const -> btDiscreteDynamicsWorld* { return world_.get(); }

    private:
        // Note: order matters for proper destruction
        uptr<btBroadphaseInterface> broadPhase_;
        uptr<btCollisionConfiguration> collisionConfig_;
        uptr<btCollisionDispatcher> collisionDispatcher_;
        uptr<btSequentialImpulseConstraintSolver> solver_;
        uptr<btDiscreteDynamicsWorld> world_;
    };
}