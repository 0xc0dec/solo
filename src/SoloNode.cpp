#include "SoloNode.h"
#include "SoloScene.h"
#include "SoloCamera.h"
#include "SoloMeshRenderer.h"
#include "SoloSkyboxRenderer.h"
#include "SoloTransform.h"
#include "SoloSpectator.h"
#include "SoloRigidBody.h"

using namespace solo;


Node::Node(Scene* scene, uint32_t nodeId):
    scene(scene), id(nodeId)
{
}


template<>
auto Node::addComponent<Transform>() -> Transform*
{
    auto transform = std::make_shared<Transform>(*this);
    scene->addComponent(id, transform);
    return transform.get();
}


template<>
auto Node::addComponent<Camera>() -> Camera*
{
    auto camera = std::make_shared<Camera>(*this);
    scene->addComponent(id, camera);
    return camera.get();
}


template<>
auto Node::addComponent<MeshRenderer>() -> MeshRenderer*
{
    auto renderer = std::make_shared<MeshRenderer>(*this);
    scene->addComponent(id, renderer);
    return renderer.get();
}


template<>
auto Node::addComponent<Spectator>() -> Spectator*
{
    auto spectator = std::make_shared<Spectator>(*this);
    scene->addComponent(id, spectator);
    return spectator.get();
}


template<>
auto Node::addComponent<SkyboxRenderer>() -> SkyboxRenderer*
{
    auto renderer = std::make_shared<SkyboxRenderer>(*this);
    scene->addComponent(id, renderer);
    return renderer.get();
}


template<>
auto Node::addComponent<RigidBody>() -> RigidBody*
{
    auto body = std::shared_ptr<RigidBody>(RigidBody::create(*this));
    scene->addComponent(id, body);
    return body.get();
}
