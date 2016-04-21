#pragma once

#include "SoloPhysics.h"
#include <btBulletDynamicsCommon.h>


namespace solo
{
    class BulletPhysics final : public Physics
    {
    public:
        virtual void update() override final;

        virtual void setGravity(const Vector3& gravity) override final;

        auto getWorld() const -> btDiscreteDynamicsWorld*;

    private:
        friend class Physics;

        BulletPhysics(Device* device, const DeviceToken& deviceToken);

        // Note: order matters for the proper destruction
        uptr<btBroadphaseInterface> broadPhase;
        uptr<btCollisionConfiguration> collisionConfig;
        uptr<btCollisionDispatcher> collisionDispatcher;
        uptr<btSequentialImpulseConstraintSolver> solver;
        uptr<btDiscreteDynamicsWorld> world;
    };
}