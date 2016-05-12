#include "SoloNode.h"
#include "SoloScene.h"
#include "SoloRigidBody.h"

using namespace solo;


Node::Node(Scene* scene, uint32_t nodeId):
    scene(scene), id(nodeId)
{
}


template <>
auto Node::addComponent(const RigidBodyConstructionParameters& params,
    std::enable_if<std::is_same<RigidBody, RigidBody>::value>::type*) -> RigidBody*
{
    auto body = std::shared_ptr<RigidBody>(RigidBody::create(*this, params));
    scene->addComponent(id, body);
    return body.get();
}
