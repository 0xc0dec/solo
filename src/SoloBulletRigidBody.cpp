#include "SoloBulletRigidBody.h"

using namespace solo;


BulletRigidBody::BulletRigidBody(Node node, const RigidBodyConstructionParameters& parameters):
    RigidBody(node)
{
    btRigidBody::btRigidBodyConstructionInfo info(parameters.mass, nullptr, nullptr/*, TODO */);
    info.m_friction = parameters.friction;
    info.m_restitution = parameters.restitution;
    info.m_linearDamping = parameters.linearDamping;
    info.m_angularDamping = parameters.angularDamping;
    
    body = std::make_unique<btRigidBody>(info);
}
