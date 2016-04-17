#include "SoloBulletPhysics.h"
#include "SoloDevice.h"

using namespace solo;


BulletPhysics::BulletPhysics(Device* device, const DeviceToken& deviceToken):
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