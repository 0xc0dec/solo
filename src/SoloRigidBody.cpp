#include "SoloRigidBody.h"
#include <btBulletDynamicsCommon.h>

using namespace solo;


class BulletRigidBody final : public RigidBody
{
public:

private:
    friend class RigidBody;

    BulletRigidBody(Node node, const RigidBodyConstructionParameters& parameters);

    uptr<btRigidBody> body;
};


BulletRigidBody::BulletRigidBody(Node node, const RigidBodyConstructionParameters& parameters) :
    RigidBody(node)
{
    btRigidBody::btRigidBodyConstructionInfo info(parameters.mass, nullptr, nullptr/*, TODO */);
    info.m_friction = parameters.friction;
    info.m_restitution = parameters.restitution;
    info.m_linearDamping = parameters.linearDamping;
    info.m_angularDamping = parameters.angularDamping;

    body = std::make_unique<btRigidBody>(info);
}


uptr<RigidBody> RigidBody::create(Node node, const RigidBodyConstructionParameters& parameters)
{
    return std::unique_ptr<RigidBody>(new BulletRigidBody(node, parameters));
}


RigidBody::RigidBody(Node node):
    ComponentBase<RigidBody>(node)
{
}
