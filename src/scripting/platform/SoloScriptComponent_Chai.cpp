#include "SoloScriptComponent_Chai.h"
#include "SoloScripter_Chai.h"
#include "SoloEngine.h"
#include "SoloDevice.h"
#include "SoloScene.h"
#include "SoloNode.h"

using namespace solo;
using namespace chaiscript;


ScriptComponent_Chai::ScriptComponent_Chai(Node* node, const std::string& componentClass) :
	ComponentBase(node)
{
	auto chai = static_cast<Scripter_Chai*>(Engine::get()->getScripter())->getEngine();
	component = chai->eval<Boxed_Value>(componentClass + "()");
	updateFunc = chai->eval<std::function<void(Boxed_Value&)>>("update");
}


void ScriptComponent_Chai::update()
{
	updateFunc(component);
}


Boxed_Value& ScriptComponent_Chai::addComponent(Boxed_Value& boxedNode, const std::string& componentClass)
{
	auto node = boxed_cast<Node*>(boxedNode);
	auto script = NEW2(ScriptComponent_Chai, node, componentClass);
	auto cmpTypeId = getHash(componentClass);
	node->getScene()->addComponent(node, script, cmpTypeId);
	return script->component;
}


void ScriptComponent_Chai::removeComponent(chaiscript::Boxed_Value& boxedNode, const std::string& componentClass)
{
	auto node = boxed_cast<Node*>(boxedNode);
	auto cmpTypeId = getHash(componentClass);
	node->getScene()->removeComponent(node, cmpTypeId);
}


Boxed_Value ScriptComponent_Chai::findComponent(Boxed_Value& boxedNode, const std::string& componentClass)
{
	auto node = boxed_cast<Node*>(boxedNode);
	auto cmpTypeId = getHash(componentClass);
	auto cmp = node->getScene()->findComponent(node, cmpTypeId);
	if (!cmp)
		return Boxed_Value();
	return static_cast<ScriptComponent_Chai*>(cmp)->component;
}