#include "SoloNode.h"
#include "SoloScene.h"
#include "SoloCamera.h"
#include "SoloMeshRenderer.h"
#include "SoloSkyboxRenderer.h"
#include "SoloTransform.h"
#include "SoloSpectator.h"

using namespace solo;


Node::Node(Scene* scene, uint32_t nodeId):
    scene(scene), id(nodeId)
{
}


template<> Transform* Node::addComponent<Transform>()
{
    auto transform = std::make_shared<Transform>(*this);
    scene->addComponent(id, transform);
    return transform.get();
}


template<> Camera* Node::addComponent<Camera>()
{
    auto camera = std::make_shared<Camera>(scene, *this);
    scene->addComponent(id, camera);
    return camera.get();
}


template<> MeshRenderer* Node::addComponent<MeshRenderer>()
{
    auto renderer = std::make_shared<MeshRenderer>(*this);
    scene->addComponent(id, renderer);
    return renderer.get();
}


template<> Spectator* Node::addComponent<Spectator>()
{
    auto spectator = std::make_shared<Spectator>(*this);
    scene->addComponent(id, spectator);
    return spectator.get();
}


template<> SkyboxRenderer* Node::addComponent<SkyboxRenderer>()
{
    auto renderer = std::make_shared<SkyboxRenderer>(*this);
    scene->addComponent(id, renderer);
    return renderer.get();
}
