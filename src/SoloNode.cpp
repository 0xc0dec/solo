#include "SoloNode.h"
#include "SoloScene.h"
#include "SoloRigidBody.h"

using namespace solo;


Node::Node(Scene* scene, uint32_t nodeId):
    scene(scene), id(nodeId)
{
}


template<>
auto Node::addComponent<RigidBody, const RigidBodyConstructionParameters&>(const RigidBodyConstructionParameters& parameters) -> RigidBody*
{
    auto body = std::shared_ptr<RigidBody>(RigidBody::create(*this, parameters));
    scene->addComponent(id, body);
    return body.get();
}
