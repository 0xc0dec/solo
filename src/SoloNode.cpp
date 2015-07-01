#include "SoloNode.h"

using namespace solo;


shared<Node> NodeFactory::createNode(Scene *scene)
{
	return NEW2(Node, scene);
}


long Node::getId() const
{
	return reinterpret_cast<long>(this);
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


template<> Script* Node::addComponent<Script>(const std::string& componentObjectName)
{
	auto script = ScriptFactory::create(this, componentObjectName);
	scene->addComponent(this, script);
	return script.get();
}