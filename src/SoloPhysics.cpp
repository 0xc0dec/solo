#include "SoloPhysics.h"
#include "SoloDevice.h"
#include <btBulletDynamicsCommon.h>

using namespace solo;


class BulletPhysics final : public Physics
{
public:
    virtual void update() override final;

    virtual void setGravity(const Vector3& gravity) override final;

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


BulletPhysics::BulletPhysics(Device* device, const DeviceToken& deviceToken) :
    Physics(device, deviceToken)
{
    broadPhase = std::make_unique<btDbvtBroadphase>();
    collisionConfig = std::make_unique<btDefaultCollisionConfiguration>();
    collisionDispatcher = std::make_unique<btCollisionDispatcher>(collisionConfig.get());
    solver = std::make_unique<btSequentialImpulseConstraintSolver>();
    world = std::make_unique<btDiscreteDynamicsWorld>(collisionDispatcher.get(), broadPhase.get(), solver.get(), collisionConfig.get());
}


void BulletPhysics::update()
{
    world->stepSimulation(device->getTimeDelta(), 7); // 7 is debatable, but good enough. See docs
}


void BulletPhysics::setGravity(const Vector3& gravity)
{
    world->setGravity(btVector3(gravity.x, gravity.y, gravity.z));
}



auto Physics::create(Device* device, const DeviceToken& deviceToken) -> uptr<Physics>
{
    return std::unique_ptr<BulletPhysics>(new BulletPhysics(device, deviceToken));
}


Physics::Physics(Device* device, const DeviceToken&):
    device(device)
{
}

