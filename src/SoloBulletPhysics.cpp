#include "SoloBulletPhysics.h"
#include "SoloDevice.h"

using namespace solo;


BulletPhysics::BulletPhysics(Device* device, const DeviceToken& deviceToken) :
    Physics(device, deviceToken)
{
    broadPhase = std::make_unique<btDbvtBroadphase>();
    collisionConfig = std::make_unique<btDefaultCollisionConfiguration>();
    collisionDispatcher = std::make_unique<btCollisionDispatcher>(collisionConfig.get());
    solver = std::make_unique<btSequentialImpulseConstraintSolver>();
    world = std::make_unique<btDiscreteDynamicsWorld>(collisionDispatcher.get(), broadPhase.get(), solver.get(), collisionConfig.get());
    world->setGravity(btVector3(0, -10, 0));
}


void BulletPhysics::update()
{
    world->stepSimulation(device->getTimeDelta(), 7); // 7 is debatable, but good enough. See docs
}


void BulletPhysics::setGravity(const Vector3& gravity)
{
    world->setGravity(btVector3(gravity.x, gravity.y, gravity.z));
}


auto BulletPhysics::getWorld() const -> btDiscreteDynamicsWorld*
{
    return world.get();
}


auto Physics::create(Device* device, const DeviceToken& deviceToken) -> sptr<Physics>
{
    return std::make_shared<BulletPhysics>(device, deviceToken);
}


Physics::Physics(Device* device, const DeviceToken&):
    device(device)
{
}
