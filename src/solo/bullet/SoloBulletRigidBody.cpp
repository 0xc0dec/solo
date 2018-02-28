/*
    Copyright (c) Aleksey Fedotov
    MIT license
*/

#include "SoloBulletRigidBody.h"
#include "SoloDevice.h"
#include "SoloBulletPhysics.h"
#include "SoloTransform.h"
#include "SoloCollider.h"
#include "SoloBulletCollider.h"
#include "SoloNode.h"
#include "SoloBulletCommon.h"

using namespace solo;

class MotionState final: public btMotionState
{
public:
    explicit MotionState(Transform *transform): transform(transform)
    {
    }

    void getWorldTransform(btTransform &worldTransform) const override final
    {
        const auto worldPos = transform->getWorldPosition();
        const auto rotation = transform->getWorldRotation();
        worldTransform.setOrigin(SL_TOBTVEC3(worldPos));
        worldTransform.setRotation(SL_TOBTQTRN(rotation));
    }

    void setWorldTransform(const btTransform &worldTransform) override final
    {
        SL_DEBUG_PANIC(transform->getParent(), "Rigid body transform must not have a parent");
        transform->setLocalPosition(SL_FROMBTVEC3(worldTransform.getOrigin()));
        transform->setLocalRotation(SL_FROMBTQTRN(worldTransform.getRotation()));
    }

private:
    Transform *transform;
};

BulletRigidBody::BulletRigidBody(const Node &node, const RigidBodyConstructionParameters &parameters):
    RigidBody(node),
    mass(parameters.mass),
    shape(nullptr)
{
    world = static_cast<BulletPhysics *>(node.getScene()->getDevice()->getPhysics())->getWorld();
    transformCmp = node.findComponent<Transform>();
    motionState = std::make_unique<MotionState>(transformCmp);

    btRigidBody::btRigidBodyConstructionInfo info(parameters.mass, motionState.get(), nullptr);
    info.m_friction = parameters.friction;
    info.m_restitution = parameters.restitution;
    info.m_linearDamping = parameters.linearDamping;
    info.m_angularDamping = parameters.angularDamping;

    body = std::make_unique<btRigidBody>(info);
    body->setUserPointer(this);
}

BulletRigidBody::~BulletRigidBody()
{
    world->removeRigidBody(body.get());
}

void BulletRigidBody::update()
{
    if (lastTransformVersion != transformCmp->getVersion())
    {
        lastTransformVersion = transformCmp->getVersion();
        if (shape)
            syncScale();
    }
}

void BulletRigidBody::setCollider(sptr<Collider> newCollider)
{
    if (newCollider)
    {
        collider = newCollider; // store ownership
        shape = std::dynamic_pointer_cast<BulletCollider>(collider)->getShape();

        btVector3 inertia;
        shape->calculateLocalInertia(mass, inertia);
        syncScale();

        body->setCollisionShape(shape);
        body->setMassProps(mass, inertia);

        world->addRigidBody(body.get());
    }
    else
    {
        world->removeRigidBody(body.get());
        collider = nullptr;
        shape = nullptr;
    }
}

bool BulletRigidBody::isKinematic()
{
    return (body->getCollisionFlags() & btCollisionObject::CF_KINEMATIC_OBJECT) == btCollisionObject::CF_KINEMATIC_OBJECT;
}

void BulletRigidBody::setKinematic(bool kinematic)
{
    auto flags = body->getCollisionFlags();
    if (kinematic)
        flags |= btCollisionObject::CF_KINEMATIC_OBJECT;
    else
        flags &= ~btCollisionObject::CF_KINEMATIC_OBJECT;
    body->setCollisionFlags(flags);
}

void BulletRigidBody::syncScale()
{
    const auto scale = transformCmp->getWorldScale();
    shape->setLocalScaling(SL_TOBTVEC3(scale));
}
