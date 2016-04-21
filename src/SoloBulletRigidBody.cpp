#include "SoloBulletRigidBody.h"

using namespace solo;


BulletRigidBody::BulletRigidBody(const Node& node, const RigidBodyConstructionParameters& parameters) :
    RigidBody(node)
{
    btRigidBody::btRigidBodyConstructionInfo info(parameters.mass, nullptr, nullptr/*, TODO */);
    info.m_friction = parameters.friction;
    info.m_restitution = parameters.restitution;
    info.m_linearDamping = parameters.linearDamping;
    info.m_angularDamping = parameters.angularDamping;

    body = std::make_unique<btRigidBody>(info);
}


auto RigidBody::create(const Node& node, const RigidBodyConstructionParameters& parameters) -> uptr<RigidBody>
{
    return std::unique_ptr<RigidBody>(new BulletRigidBody(node, parameters));
}


RigidBody::RigidBody(const Node& node):
    ComponentBase<RigidBody>(node)
{
}
