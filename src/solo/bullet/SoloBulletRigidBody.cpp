/* 
 * Copyright (c) Aleksey Fedotov 
 * MIT license 
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
    explicit MotionState(Transform *transform): transform_(transform)
    {
    }

    void getWorldTransform(btTransform &worldTransform) const override final
    {
        const auto worldPos = transform_->worldPosition();
        const auto rotation = transform_->worldRotation();
        worldTransform.setOrigin(SL_TOBTVEC3(worldPos));
        worldTransform.setRotation(SL_TOBTQTRN(rotation));
    }

    void setWorldTransform(const btTransform &worldTransform) override final
    {
        SL_DEBUG_PANIC(transform_->parent(), "Rigid body transform must not have a parent");
        transform_->setLocalPosition(SL_FROMBTVEC3(worldTransform.getOrigin()));
        transform_->setLocalRotation(SL_FROMBTQTRN(worldTransform.getRotation()));
    }

private:
    Transform *transform_;
};

BulletRigidBody::BulletRigidBody(const Node &node, const RigidBodyParams &params):
    RigidBody(node),
    mass_(params.mass),
    shape_(nullptr)
{
    world_ = static_cast<BulletPhysics *>(node.scene()->device()->physics())->world();
    transformCmp_ = node.findComponent<Transform>();
    motionState_ = std::make_unique<MotionState>(transformCmp_);

    btRigidBody::btRigidBodyConstructionInfo info(params.mass, motionState_.get(), nullptr);
    info.m_friction = params.friction;
    info.m_restitution = params.restitution;
    info.m_linearDamping = params.linearDamping;
    info.m_angularDamping = params.angularDamping;

    body_ = std::make_unique<btRigidBody>(info);
    body_->setUserPointer(this);
}

BulletRigidBody::~BulletRigidBody()
{
    world_->removeRigidBody(body_.get());
}

void BulletRigidBody::update()
{
    if (lastTransformVersion_ != transformCmp_->version())
    {
        lastTransformVersion_ = transformCmp_->version();
        if (shape_)
            syncScale();
    }
}

void BulletRigidBody::setCollider(sptr<Collider> newCollider)
{
    if (newCollider)
    {
        collider_ = newCollider; // store ownership
        shape_ = std::dynamic_pointer_cast<BulletCollider>(collider_)->shape();

        btVector3 inertia;
        if (!shape_->isNonMoving())
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
    return (body_->getCollisionFlags() & btCollisionObject::CF_KINEMATIC_OBJECT)
        == btCollisionObject::CF_KINEMATIC_OBJECT;
}

void BulletRigidBody::setKinematic(bool kinematic)
{
    auto flags = body_->getCollisionFlags();
    if (kinematic)
    {
        flags |= btCollisionObject::CF_KINEMATIC_OBJECT;
        body_->setActivationState(DISABLE_DEACTIVATION);
    }
    else
    {
        flags &= ~btCollisionObject::CF_KINEMATIC_OBJECT;
        body_->setActivationState(WANTS_DEACTIVATION); // TODO not sure the right flag
    }
    body_->setCollisionFlags(flags);
}

void BulletRigidBody::syncScale()
{
    const auto scale = transformCmp_->worldScale();
    shape_->setLocalScaling(SL_TOBTVEC3(scale));
}
