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

        auto castRay(const Vector3 &from, const Vector3 &to) -> RaycastResult override final;
        auto castRayAll(const Vector3 &from, const Vector3 &to) -> vec<RaycastResult> override final;

        auto getWorld() const -> btDiscreteDynamicsWorld* { return world.get(); }

    private:
        // Note: order matters for proper destruction
        uptr<btBroadphaseInterface> broadPhase;
        uptr<btCollisionConfiguration> collisionConfig;
        uptr<btCollisionDispatcher> collisionDispatcher;
        uptr<btSequentialImpulseConstraintSolver> solver;
        uptr<btDiscreteDynamicsWorld> world;
    };
}