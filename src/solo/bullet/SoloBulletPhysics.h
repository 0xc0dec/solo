/*
    Copyright (c) Aleksey Fedotov
    MIT license
*/

#pragma once

#include "SoloPhysics.h"
#include <btBulletDynamicsCommon.h>

namespace solo
{
    namespace bullet
    {
        class Physics final : public solo::Physics
        {
        public:
            Physics(Device *device, const FriendToken<Device> &deviceToken);

            void update() override final;

            void setGravity(const Vector3 &gravity) override final;

            auto castRay(const Vector3 &from, const Vector3 &to) -> RaycastResult override;
            auto castRayAll(const Vector3 &from, const Vector3 &to) -> vec<RaycastResult> override;

            auto getWorld() const -> btDiscreteDynamicsWorld*;

        private:
            // Note: order matters for the proper destruction
            uptr<btBroadphaseInterface> broadPhase;
            uptr<btCollisionConfiguration> collisionConfig;
            uptr<btCollisionDispatcher> collisionDispatcher;
            uptr<btSequentialImpulseConstraintSolver> solver;
            uptr<btDiscreteDynamicsWorld> world;
        };
    }
}