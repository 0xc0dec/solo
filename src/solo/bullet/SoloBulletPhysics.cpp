/* 
 * Copyright (c) Aleksey Fedotov 
 * MIT license 
 */

#include "SoloBulletPhysics.h"
#include "SoloDevice.h"
#include "SoloBulletCommon.h"

using namespace solo;

BulletPhysics::BulletPhysics(Device *device):
    Physics(device)
{
    broadPhase_ = std::make_unique<btDbvtBroadphase>();
    collisionConfig_ = std::make_unique<btDefaultCollisionConfiguration>();
    collisionDispatcher_ = std::make_unique<btCollisionDispatcher>(collisionConfig_.get());
    solver_ = std::make_unique<btSequentialImpulseConstraintSolver>();
    world_ = std::make_unique<btDiscreteDynamicsWorld>(collisionDispatcher_.get(), broadPhase_.get(), solver_.get(), collisionConfig_.get());
    world_->setGravity(btVector3(0, -10, 0));
}

void BulletPhysics::update()
{
    world_->stepSimulation(device_->timeDelta(), 7); // 7 is debatable, but good enough. See docs
}

void BulletPhysics::setGravity(const Vector3 &gravity)
{
    world_->setGravity(SL_TOBTVEC3(gravity));
}

auto BulletPhysics::rayTestFirst(const Vector3 &from, const Vector3 &to) -> RayTestResult
{
    const auto btFrom = SL_TOBTVEC3(from);
    const auto btTo = SL_TOBTVEC3(to);

    btCollisionWorld::ClosestRayResultCallback callback(btFrom, btTo);
    world_->rayTest(btFrom, btTo, callback);
    if (!callback.hasHit())
        return RayTestResult();

    const auto body = dynamic_cast<const btRigidBody *>(callback.m_collisionObject);
    if (!body)
        return RayTestResult();

    const auto rigidBody = static_cast<RigidBody *>(body->getUserPointer());
    return RayTestResult(rigidBody, SL_FROMBTVEC3(callback.m_hitPointWorld), SL_FROMBTVEC3(callback.m_hitNormalWorld));
}

auto BulletPhysics::rayTestAll(const Vector3 &from, const Vector3 &to) -> vec<RayTestResult>
{
    const auto btFrom = SL_TOBTVEC3(from);
    const auto btTo = SL_TOBTVEC3(to);

    btCollisionWorld::AllHitsRayResultCallback callback(btFrom, btTo);
    world_->rayTest(btFrom, btTo, callback);
    const auto count = callback.m_collisionObjects.size();
    if (!count)
        return {};

    auto result = vec<RayTestResult>();
    for (size_t i = 0; i < count; i++)
    {
        const auto body = dynamic_cast<const btRigidBody *>(callback.m_collisionObjects[i]);
        if (body)
        {
            const auto rigidBody = static_cast<RigidBody *>(body->getUserPointer());
            result.emplace_back(rigidBody, SL_FROMBTVEC3(callback.m_hitPointWorld[i]), SL_FROMBTVEC3(callback.m_hitNormalWorld[i]));
        }
    }

    return result;
}
