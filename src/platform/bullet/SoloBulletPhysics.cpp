#include "SoloBulletPhysics.h"
#include "SoloDevice.h"
#include "SoloBulletCommon.h"

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
    world->setGravity(SL_TOBTVEC3(gravity));
}


auto BulletPhysics::castRay(const Vector3& from, const Vector3& to) -> RaycastResult
{
    auto btFrom = SL_TOBTVEC3(from);
    auto btTo = SL_TOBTVEC3(to);
    btCollisionWorld::ClosestRayResultCallback callback(btFrom, btTo);
    world->rayTest(btFrom, btTo, callback);

    RaycastResult result;
    auto rigidBody = dynamic_cast<const btRigidBody*>(callback.m_collisionObject);
    result.anyHit = rigidBody != nullptr;
    if (result.anyHit)
    {
        result.rigidBody = static_cast<RigidBody*>(rigidBody->getUserPointer());
        // TODO
    }
    return result;
}


auto BulletPhysics::getWorld() const -> btDiscreteDynamicsWorld*
{
    return world.get();
}
