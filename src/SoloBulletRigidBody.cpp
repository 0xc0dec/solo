#include "SoloBulletRigidBody.h"
#include "SoloDevice.h"
#include "SoloBulletPhysics.h"
#include "SoloTransform.h"
#include "SoloCollider.h"
#include "SoloBulletCollider.h"
#include "SoloNode.h"

using namespace solo;


BulletRigidBody::BulletRigidBody(const Node& node, const RigidBodyConstructionParameters& parameters) :
    RigidBody(node),
    mass(parameters.mass)
{
    world = static_cast<BulletPhysics*>(Device::get()->getPhysics())->getWorld();
    transformCmp = node.getComponent<Transform>();

    // TODO move to a method
    auto worldPos = transformCmp->getWorldPosition();
    auto rot = transformCmp->getWorldRotation();
    btTransform transform;
    transform.setOrigin(btVector3(worldPos.x, worldPos.y, worldPos.z));
    transform.setRotation(btQuaternion(rot.x, rot.y, rot.z, rot.w));
    motionState = std::make_unique<btDefaultMotionState>(transform);

    btRigidBody::btRigidBodyConstructionInfo info(parameters.mass, motionState.get(), nullptr);
    info.m_friction = parameters.friction;
    info.m_restitution = parameters.restitution;
    info.m_linearDamping = parameters.linearDamping;
    info.m_angularDamping = parameters.angularDamping;

    body = std::make_unique<btRigidBody>(info);
}


BulletRigidBody::~BulletRigidBody()
{
    world->removeRigidBody(body.get());
}


void BulletRigidBody::setCollider(sptr<Collider> newCollider)
{
    if (newCollider)
    {
        collider = newCollider;

        auto shape = std::dynamic_pointer_cast<BulletCollider>(collider)->getShape();
        btVector3 inertia;
        shape->calculateLocalInertia(mass, inertia);

        body->setCollisionShape(shape);
        body->setMassProps(mass, inertia);

        world->addRigidBody(body.get());
    }
    else
    {
        world->removeRigidBody(body.get());
        collider = nullptr;
    }
}


void BulletRigidBody::update() // TODO this should be done before update
{
    btTransform transform;
    motionState->getWorldTransform(transform);
    auto origin = transform.getOrigin();
    auto rotation = transform.getRotation();
    transformCmp->setWorldPosition(Vector3(origin.x(), origin.y(), origin.z()));
    transformCmp->setLocalRotation(Quaternion(rotation.x(), rotation.y(), rotation.z(), rotation.w()));
}


auto RigidBody::create(const Node& node, const RigidBodyConstructionParameters& parameters) -> sptr<RigidBody>
{
    return std::make_shared<BulletRigidBody>(node, parameters);
}


RigidBody::RigidBody(const Node& node):
    ComponentBase<RigidBody>(node)
{
}
