#include "SoloRigidBody.h"
#include "SoloBulletRigidBody.h"

using namespace solo;


uptr<RigidBody> RigidBody::create(Node node, const RigidBodyConstructionParameters& parameters)
{
    return std::unique_ptr<RigidBody>(new BulletRigidBody(node, parameters));
}


RigidBody::RigidBody(Node node):
    ComponentBase<RigidBody>(node)
{
}
