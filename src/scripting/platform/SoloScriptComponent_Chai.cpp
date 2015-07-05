#include "SoloScriptComponent_Chai.h"
#include "SoloScripter_Chai.h"
#include "SoloEngine.h"
#include "SoloDevice.h"
#include "SoloScene.h"
#include "SoloNode.h"

using namespace solo;


ScriptComponent_Chai::ScriptComponent_Chai(Node* node, const std::string& componentClass) :
	ComponentBase(node)
{
	auto engine = Engine::get();
	auto chai = static_cast<Scripter_Chai*>(engine->getScripter())->getEngine();
	device = engine->getDevice();
	component = chai->eval<chaiscript::Boxed_Value>(componentClass + "()");
	updateFunc = chai->eval<std::function<void(chaiscript::Boxed_Value&, float)>>("update");
}


void ScriptComponent_Chai::update()
{
	updateFunc(component, device->getTimeDelta());
}


chaiscript::Boxed_Value& ScriptComponent_Chai::addComponent(chaiscript::Boxed_Value& node, const std::string& componentClass)
{
	auto n = chaiscript::boxed_cast<Node*>(node);
	auto script = NEW2(ScriptComponent_Chai, n, componentClass);
	n->getScene()->addComponent(n, script);
	return script->component;
}