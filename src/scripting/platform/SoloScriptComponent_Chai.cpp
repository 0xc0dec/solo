#include "SoloCamera.h"
#include "SoloScriptComponent_Chai.h"
#include "SoloScripter_Chai.h"
#include "SoloEngine.h"
#include "SoloScene.h"
#include "SoloNode.h"
#include "SoloTransform.h"
#include "SoloModelRenderer.h"

using namespace solo;
using namespace chaiscript;


ScriptComponent_Chai::ScriptComponent_Chai(Node* node, const std::string& componentClass) :
	ComponentBase(node)
{
	auto scriptEngine = static_cast<Scripter_Chai*>(Engine::get()->getScripter())->getEngine();
	auto constructor = scriptEngine->eval<std::function<Boxed_Value(Node*)>>(componentClass);
	component = constructor(node);
	updateFunc = scriptEngine->eval<std::function<void(Boxed_Value&)>>("update");
}


void ScriptComponent_Chai::update()
{
	updateFunc(component);
}


Boxed_Value ScriptComponent_Chai::addComponent(Boxed_Value& boxedNode, const std::string& componentClass)
{
	auto node = boxed_cast<Node*>(boxedNode);
	if (componentClass == "Camera")
		return Boxed_Value(node->addComponent<Camera>());
	if (componentClass == "ModelRenderer")
		return Boxed_Value(node->addComponent<ModelRenderer>());

	auto script = NEW2(ScriptComponent_Chai, node, componentClass);
	auto cmpTypeId = getHash(componentClass);
	node->getScene()->addComponent(node, script, cmpTypeId);
	return script->component;
}


void ScriptComponent_Chai::removeComponent(Boxed_Value& boxedNode, const std::string& componentClass)
{
	auto node = boxed_cast<Node*>(boxedNode);
	auto cmpTypeId = getHash(componentClass);
	node->getScene()->removeComponent(node, cmpTypeId);
}


Boxed_Value ScriptComponent_Chai::findComponent(Boxed_Value& boxedNode, const std::string& componentClass)
{
	auto node = boxed_cast<Node*>(boxedNode);
	if (componentClass == "Transform")
		return Boxed_Value(node->findComponent<Transform>());
	if (componentClass == "Camera")
		return Boxed_Value(node->findComponent<Camera>());
	if (componentClass == "ModelRenderer")
		return Boxed_Value(node->findComponent<ModelRenderer>());

	auto cmpTypeId = getHash(componentClass);
	auto cmp = node->getScene()->findComponent(node, cmpTypeId);
	return cmp ? static_cast<ScriptComponent_Chai*>(cmp)->component : Boxed_Value();
}