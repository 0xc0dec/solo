#include "SoloNode.h"
#include "SoloEngine.h"
#include "SoloModelRenderer.h"
#include "SoloCamera.h"
#include "SoloTransform.h"

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


template<> Script* Node::addComponent<Script>()
{
	auto script = ScriptFactory::create(this);
	scene->addComponent(this, script);
	return script.get();
}