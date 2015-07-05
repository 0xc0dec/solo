#include "SoloScriptComponent_Lua.h"
#include "SoloEngine.h"
#include "SoloScripter_Lua.h"
#include "SoloDevice.h"
#include "SoloTransform.h"
#include "SoloCamera.h"

using namespace solo;


ScriptComponent_Lua::ScriptComponent_Lua(Node* node, const std::string& expression):
	ComponentBase(node)
{
	auto engine = static_cast<Scripter_Lua*>(Engine::get()->getScripter())->getEngine();
	device = Engine::get()->getDevice();
	updateFunc = engine->executeCode<decltype(updateFunc)>("return " + expression + ".update");
}


void ScriptComponent_Lua::update()
{
	updateFunc(device->getTimeDelta());
}


void ScriptComponent_Lua::addComponent(Node& node, const std::string& expression)
{
	auto engine = static_cast<Scripter_Lua*>(Engine::get()->getScripter())->getEngine();
	auto typeId = getHash(engine->executeCode<const std::string&>("return " + expression + ".id"));
	auto script = NEW2(ScriptComponent_Lua, &node, expression);
	node.getScene()->addComponent(&node, script, typeId);
}


void ScriptComponent_Lua::removeComponent(Node& node, const std::string& expression)
{
	auto engine = static_cast<Scripter_Lua*>(Engine::get()->getScripter())->getEngine();
	auto typeId = getHash(engine->executeCode<const std::string&>("return " + expression + ".id"));
	node.getScene()->removeComponent(&node, typeId);
}


boost::variant<Transform*, Camera*> ScriptComponent_Lua::findComponent(Node& node, const std::string& name)
{
	if (name == "Transform")
		return node.findComponent<Transform>();
	if (name == "Camera")
		return node.findComponent<Camera>();
	return static_cast<Transform*>(nullptr);
}

//
//void ScriptComponent_Lua::findComponent(Node& node, const std::string& name)
//{
//	if (name == "Transform")
//		return node.getComponent<Transform>();
//}