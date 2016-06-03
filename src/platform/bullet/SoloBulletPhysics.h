#pragma once

#include "SoloPhysics.h"
#include <btBulletDynamicsCommon.h>


namespace solo
{
    class BulletPhysics final : public Physics
    {
    public:
        BulletPhysics(Device* device, const DeviceToken& deviceToken);

        void update() override final;

        void setGravity(const Vector3& gravity) override final;

        auto castRay(const Vector3& from, const Vector3& to) -> RaycastResult override;
        auto castRayAll(const Vector3& from, const Vector3& to) -> std::vector<RaycastResult> override;

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