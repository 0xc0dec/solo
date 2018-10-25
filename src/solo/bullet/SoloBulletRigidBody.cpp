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
    mass_(parameters.mass),
    shape_(nullptr)
{
    world_ = static_cast<BulletPhysics *>(node.getScene()->getDevice()->getPhysics())->world();
    transformCmp_ = node.findComponent<Transform>();
    motionState_ = std::make_unique<MotionState>(transformCmp_);

    btRigidBody::btRigidBodyConstructionInfo info(parameters.mass, motionState_.get(), nullptr);
    info.m_friction = parameters.friction;
    info.m_restitution = parameters.restitution;
    info.m_linearDamping = parameters.linearDamping;
    info.m_angularDamping = parameters.angularDamping;

    body_ = std::make_unique<btRigidBody>(info);
    body_->setUserPointer(this);
}

BulletRigidBody::~BulletRigidBody()
{
    world_->removeRigidBody(body_.get());
}

void BulletRigidBody::update()
{
    if (lastTransformVersion_ != transformCmp_->getVersion())
    {
        lastTransformVersion_ = transformCmp_->getVersion();
        if (shape_)
            syncScale();
    }
}

void BulletRigidBody::setCollider(sptr<Collider> newCollider)
{
    if (newCollider)
    {
        collider_ = newCollider; // store ownership
        shape_ = std::dynamic_pointer_cast<BulletCollider>(collider_)->getShape();

        btVector3 inertia;
        shape_->calculateLocalInertia(mass_, inertia);
        syncScale();

        body_->setCollisionShape(shape_);
        body_->setMassProps(mass_, inertia);

        world_->addRigidBody(body_.get());
    }
    else
    {
        world_->removeRigidBody(body_.get());
        collider_ = nullptr;
        shape_ = nullptr;
    }
}

bool BulletRigidBody::isKinematic()
{
    return (body_->getCollisionFlags() & btCollisionObject::CF_KINEMATIC_OBJECT) == btCollisionObject::CF_KINEMATIC_OBJECT;
}

void BulletRigidBody::setKinematic(bool kinematic)
{
    auto flags = body_->getCollisionFlags();
    if (kinematic)
        flags |= btCollisionObject::CF_KINEMATIC_OBJECT;
    else
        flags &= ~btCollisionObject::CF_KINEMATIC_OBJECT;
    body_->setCollisionFlags(flags);
}

void BulletRigidBody::syncScale()
{
    const auto scale = transformCmp_->getWorldScale();
    shape_->setLocalScaling(SL_TOBTVEC3(scale));
}
