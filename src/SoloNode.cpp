#include "SoloNode.h"
#include "SoloScene.h"
#include "SoloCamera.h"
#include "SoloModelRenderer.h"
#include "SoloTransform.h"
#include "SoloEngine.h"

using namespace solo;


template<> Transform* Node::addComponent<Transform>()
{
	auto transform = TransformFactory::create(*this);
	scene->addComponent(id, transform);
	return transform.get();
}


template<> Camera* Node::addComponent<Camera>()
{
	auto camera = Camera::create(scene->getEngine()->getMode(), scene, *this);
	scene->addComponent(id, camera);
	return camera.get();
}


template<> ModelRenderer* Node::addComponent<ModelRenderer>()
{
	auto renderer = ModelRendererFactory::create(*this);
	scene->addComponent(id, renderer);
	return renderer.get();
}
