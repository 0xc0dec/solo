/*
    Copyright (c) Aleksey Fedotov
    MIT license
*/

#include "SoloBulletPhysics.h"
#include "SoloDevice.h"
#include "SoloBulletCommon.h"

using namespace solo;

bullet::Physics::Physics(Device *device, const FriendToken<Device> &deviceToken) :
    solo::Physics(device, deviceToken)
{
    broadPhase = std::make_unique<btDbvtBroadphase>();
    collisionConfig = std::make_unique<btDefaultCollisionConfiguration>();
    collisionDispatcher = std::make_unique<btCollisionDispatcher>(collisionConfig.get());
    solver = std::make_unique<btSequentialImpulseConstraintSolver>();
    world = std::make_unique<btDiscreteDynamicsWorld>(collisionDispatcher.get(), broadPhase.get(), solver.get(), collisionConfig.get());
    world->setGravity(btVector3(0, -10, 0));
}

void bullet::Physics::update()
{
    world->stepSimulation(device->getTimeDelta(), 7); // 7 is debatable, but good enough. See docs
}

void bullet::Physics::setGravity(const Vector3 &gravity)
{
    world->setGravity(SL_TOBTVEC3(gravity));
}

auto bullet::Physics::castRay(const Vector3 &from, const Vector3 &to) -> RaycastResult
{
    auto btFrom = SL_TOBTVEC3(from);
    auto btTo = SL_TOBTVEC3(to);

    btCollisionWorld::ClosestRayResultCallback callback(btFrom, btTo);
    world->rayTest(btFrom, btTo, callback);
    if (!callback.hasHit())
        return RaycastResult();

    auto body = dynamic_cast<const btRigidBody *>(callback.m_collisionObject);
    if (!body)
        return RaycastResult();

    auto rigidBody = static_cast<RigidBody *>(body->getUserPointer());
    return RaycastResult(rigidBody, SL_FROMBTVEC3(callback.m_hitPointWorld), SL_FROMBTVEC3(callback.m_hitNormalWorld));
}

auto bullet::Physics::castRayAll(const Vector3 &from, const Vector3 &to) -> std::vector<RaycastResult>
{
    auto btFrom = SL_TOBTVEC3(from);
    auto btTo = SL_TOBTVEC3(to);

    btCollisionWorld::AllHitsRayResultCallback callback(btFrom, btTo);
    world->rayTest(btFrom, btTo, callback);
    auto size = callback.m_collisionObjects.size();
    if (size == 0)
        return {};

    auto result = std::vector<RaycastResult>();
    for (size_t i = 0; i < size; i++)
    {
        auto body = dynamic_cast<const btRigidBody *>(callback.m_collisionObjects[i]);
        if (!body)
            continue;
        auto rigidBody = static_cast<RigidBody *>(body->getUserPointer());
        result.push_back(RaycastResult(rigidBody, SL_FROMBTVEC3(callback.m_hitPointWorld[i]), SL_FROMBTVEC3(callback.m_hitNormalWorld[i])));
    }

    return result;
}

auto bullet::Physics::getWorld() const -> btDiscreteDynamicsWorld*
{
    return world.get();
}
