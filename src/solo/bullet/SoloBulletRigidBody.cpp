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

class MotionState final: public btMotionState {
public:
    explicit MotionState(Transform *transform): transform_(transform) {
    }

    void getWorldTransform(btTransform &worldTransform) const override {
        const auto worldPos = transform_->worldPosition();
        const auto rotation = transform_->worldRotation();
        worldTransform.setOrigin(SL_TOBTVEC3(worldPos));
        worldTransform.setRotation(SL_TOBTQTRN(rotation));
    }

    void setWorldTransform(const btTransform &worldTransform) override {
        panicIf(transform_->parent(), "Rigid body transform must not have a parent");
        transform_->setLocalPosition(SL_FROMBTVEC3(worldTransform.getOrigin()));
        transform_->setLocalRotation(SL_FROMBTQTRN(worldTransform.getRotation()));
    }

private:
    Transform *transform_;
};

BulletRigidBody::BulletRigidBody(const Node &node, const RigidBodyParams &params):
    RigidBody(node),
    mass_(params.mass),
    shape_(nullptr) {
    world_ = dynamic_cast<BulletPhysics *>(node.scene()->device()->physics())->world();
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

BulletRigidBody::~BulletRigidBody() {
    world_->removeRigidBody(body_.get());
}

void BulletRigidBody::update() {
    if (lastTransformVersion_ != transformCmp_->version()) {
        lastTransformVersion_ = transformCmp_->version();
        if (isStatic())
            syncPosition();
        if (shape_)
            syncScale();
    }
}

void BulletRigidBody::setCollider(sptr<Collider> newCollider) {
    if (newCollider) {
        collider_ = newCollider; // store ownership
        shape_ = std::dynamic_pointer_cast<BulletCollider>(collider_)->shape();

        btVector3 inertia;
        if (!shape_->isNonMoving())
            shape_->calculateLocalInertia(mass_, inertia);

        syncScale();

        body_->setCollisionShape(shape_);
        body_->setMassProps(mass_, inertia);

        world_->addRigidBody(body_.get());
    } else {
        world_->removeRigidBody(body_.get());
        collider_.reset();
        shape_ = nullptr;
    }
}

bool BulletRigidBody::isKinematic() {
    return body_->getCollisionFlags() & btCollisionObject::CF_KINEMATIC_OBJECT;
}

void BulletRigidBody::setKinematic(bool kinematic) {
    auto flags = body_->getCollisionFlags();
    if (kinematic) {
        flags |= btCollisionObject::CF_KINEMATIC_OBJECT;
        body_->setActivationState(DISABLE_DEACTIVATION);
    } else {
        flags &= ~btCollisionObject::CF_KINEMATIC_OBJECT;
        body_->setActivationState(WANTS_DEACTIVATION); // TODO not sure the right flag
    }
    body_->setCollisionFlags(flags);
}

bool BulletRigidBody::isStatic() {
    return body_->getCollisionFlags() & btCollisionObject::CF_STATIC_OBJECT;
}

void BulletRigidBody::setStatic(bool isStatic) {
    auto flags = body_->getCollisionFlags();
    if (isStatic)
        flags |= btCollisionObject::CF_STATIC_OBJECT;
    else
        flags &= ~btCollisionObject::CF_STATIC_OBJECT;
    body_->setCollisionFlags(flags);
}

void BulletRigidBody::syncScale() const {
    const auto scale = transformCmp_->worldScale();
    shape_->setLocalScaling(SL_TOBTVEC3(scale));
}

void BulletRigidBody::syncPosition() const {
    const auto pos = transformCmp_->worldPosition();
    const auto rot = transformCmp_->worldRotation();
    const auto transform = btTransform(SL_TOBTQTRN(rot), SL_TOBTVEC3(pos));
    body_->setWorldTransform(transform);
}
