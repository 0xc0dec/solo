#include "SoloNode.h"
#include "SoloScene.h"
#include "SoloCamera.h"
#include "SoloDevice.h"
#include "SoloModelRenderer.h"
#include "SoloTransform.h"

using namespace solo;


Node::Node(Scene* scene, size_t nodeId) :
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


template<> ModelRenderer* Node::addComponent<ModelRenderer>()
{
	auto renderer = ModelRenderer::create(*this);
	scene->addComponent(id, renderer);
	return renderer.get();
}
