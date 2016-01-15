#include "SoloNode.h"
#include "SoloScene.h"
#include "SoloCamera.h"
#include "SoloDevice.h"
#include "SoloMeshRenderer.h"
#include "SoloSkyboxRenderer.h"
#include "SoloTransform.h"
#include "SoloSpectator.h"

using namespace solo;


Node::Node(Scene* scene, int nodeId):
    scene(scene), id(nodeId)
{
}


template<> Transform* Node::addComponent<Transform>()
{
    auto transform = Transform::create(*this);
    scene->addComponent(id, transform);
    return transform.get();
}


template<> Camera* Node::addComponent<Camera>()
{
    auto camera = Camera::create(scene->getDevice()->getMode(), scene, *this);
    scene->addComponent(id, camera);
    return camera.get();
}


template<> MeshRenderer* Node::addComponent<MeshRenderer>()
{
    auto renderer = MeshRenderer::create(*this);
    scene->addComponent(id, renderer);
    return renderer.get();
}


template<> Spectator* Node::addComponent<Spectator>()
{
    auto spectator = Spectator::create(*this);
    scene->addComponent(id, spectator);
    return spectator.get();
}


template<> SkyboxRenderer* Node::addComponent<SkyboxRenderer>()
{
    auto renderer = SkyboxRenderer::create(*this);
    scene->addComponent(id, renderer);
    return renderer.get();
}