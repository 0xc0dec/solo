/*
    Copyright (c) Aleksey Fedotov

    This software is provided 'as-is', without any express or implied
    warranty. In no event will the authors be held liable for any damages
    arising from the use of this software.

    Permission is granted to anyone to use this software for any purpose,
    including commercial applications, and to alter it and redistribute it
    freely, subject to the following restrictions:

    1. The origin of this software must not be misrepresented; you must not
        claim that you wrote the original software. If you use this software
        in a product, an acknowledgment in the product documentation would be
        appreciated but is not required.
    2. Altered source versions must be plainly marked as such, and must not be
        misrepresented as being the original software.
    3. This notice may not be removed or altered from any source distribution.
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
    explicit MotionState(Transform* transform): transform(transform)
    {
    }

    void getWorldTransform(btTransform& worldTransform) const override final
    {
        auto worldPos = transform->getWorldPosition();
        auto rotation = transform->getWorldRotation();
        worldTransform.setOrigin(SL_TOBTVEC3(worldPos));
        worldTransform.setRotation(SL_TOBTQTRN(rotation));
    }

    void setWorldTransform(const btTransform& worldTransform) override final
    {
        SL_ERR_IF(transform->getParent())
        transform->setLocalPosition(SL_FROMBTVEC3(worldTransform.getOrigin()));
        transform->setLocalRotation(SL_FROMBTQTRN(worldTransform.getRotation()));
    }

private:
    Transform* transform;
};


BulletRigidBody::BulletRigidBody(const Node& node, const RigidBodyConstructionParameters& parameters):
    RigidBody(node),
    mass(parameters.mass),
    shape(nullptr)
{
    world = static_cast<BulletPhysics*>(node.getScene()->getDevice()->getPhysics())->getWorld();
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


void BulletRigidBody::onTransformChanged(const Transform* transform, uint32_t dirtyFlags)
{
    if (shape && dirtyFlags | TransformDirtyFlags::Scale)
        syncScale();
}


void BulletRigidBody::syncScale()
{
    auto scale = transformCmp->getLocalScale();
    shape->setLocalScaling(SL_TOBTVEC3(scale));
}
