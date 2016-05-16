#include "SoloNode.h"
#include "SoloScene.h"
#include "SoloRigidBody.h"

using namespace solo;


Node::Node(Scene* scene, uint32_t nodeId):
    scene(scene), id(nodeId)
{
}

//template <>
//template <class ... Args>
//auto NodeDefaultComponentHelper<RigidBody>::addComponent(uint32_t nodeId, Scene* scene, Args&&... args) -> RigidBody*
//{
//    auto body = std::shared_ptr<RigidBody>(RigidBody::create(*this, std::forward<Args>(args)...));
//    scene->addComponent(id, body);
//    return body.get();
//}

//
//template <>
//auto Node::addComponent(const RigidBodyConstructionParameters& params,
//    std::enable_if<std::is_same<RigidBody, RigidBody>::value>::type*) -> RigidBody*
//{
//    auto body = std::shared_ptr<RigidBody>(RigidBody::create(*this, params));
//    scene->addComponent(id, body);
//    return body.get();
//}
