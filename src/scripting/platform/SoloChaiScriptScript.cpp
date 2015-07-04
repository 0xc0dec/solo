#include "SoloChaiScriptScript.h"
#include "SoloEngine.h"
#include "SoloDevice.h"
#include "SoloChaiScriptScripter.h"
#include "SoloScene.h"
#include "SoloNode.h"

using namespace solo;


ChaiScriptComponent::ChaiScriptComponent(Node* node, chaiscript::Boxed_Value obj) :
	ComponentBase(node),
	component(obj)
{
	auto engine = Engine::get();
	device = engine->getDevice();
	auto scripter = static_cast<ChaiScriptScripter*>(engine->getScripter());
	updateFunc = scripter->getEngine()->eval<std::function<void(chaiscript::Boxed_Value&, float)>>("update");
}


void ChaiScriptComponent::update()
{
	updateFunc(component, device->getTimeDelta());
}


ChaiScriptComponent* ChaiScriptComponent::addComponent(chaiscript::Boxed_Value& node, chaiscript::Boxed_Value& component)
{
	auto n = chaiscript::boxed_cast<Node*>(node);
	auto script = NEW2(ChaiScriptComponent, n, component);
	Engine::get()->getScene()->addComponent(n, script);
	return script.get();
}