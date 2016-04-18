#include "SoloRigidBody.h"
#include "SoloBulletRigidBody.h"

using namespace solo;


uptr<RigidBody> RigidBody::create(Node node)
{
    return std::unique_ptr<RigidBody>(new BulletRigidBody(node));
}


RigidBody::RigidBody(Node node):
    ComponentBase<RigidBody>(node)
{
}
