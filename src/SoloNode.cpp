#include "SoloNode.h"

using namespace solo;


ptr<Node> NodeFactory::createNode(Scene* scene)
{
	return NEW2(Node, scene);
}


template<> Transform* Node::addComponent<Transform>()
{
	auto transform = TransformFactory::create(this);
	scene->addComponent(this, transform);
	return transform.get();
}


template<> Camera* Node::addComponent<Camera>()
{
	auto camera = CameraFactory::create(this);
	scene->addComponent(this, camera);
	return camera.get();
}


template<> ModelRenderer* Node::addComponent<ModelRenderer>()
{
	auto renderer = ModelRendererFactory::create(this);
	scene->addComponent(this, renderer);
	return renderer.get();
}