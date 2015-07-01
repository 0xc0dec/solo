#include "SoloNode.h"
#include "SoloEngine.h"

using namespace solo;


shared<Node> NodeFactory::createNode()
{
	return NEW2(Node);
}


Node::Node()
{
	scene = Engine::get()->getScene();
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


template<> Script* Node::addComponent<Script>(const std::string& callbackObjectName)
{
	auto script = ScriptFactory::create(this, callbackObjectName);
	scene->addComponent(this, script);
	return script.get();
}