#include "SoloScriptComponent_Lua.h"
#include "SoloEngine.h"
#include "SoloScripter_Lua.h"
#include "SoloDevice.h"

using namespace solo;


ScriptComponent_Lua::ScriptComponent_Lua(Node* node, const std::string& componentName):
	ComponentBase(node)
{
	auto engine = static_cast<Scripter_Lua*>(Engine::get()->getScripter())->getEngine();
	device = Engine::get()->getDevice();
	updateFunc = engine->executeCode<decltype(updateFunc)>("return " + componentName + ".update");
}


void ScriptComponent_Lua::update()
{
	updateFunc(device->getTimeDelta());
}


void ScriptComponent_Lua::addComponent(Node& node, const std::string& componentName)
{
	auto engine = static_cast<Scripter_Lua*>(Engine::get()->getScripter())->getEngine();
	auto typeId = getHash(engine->executeCode<const std::string&>("return " + componentName + ".id"));
	auto script = NEW2(ScriptComponent_Lua, &node, componentName);
	node.getScene()->addComponent(&node, script, typeId);
}


void ScriptComponent_Lua::removeComponent(Node& node, const std::string& componentName)
{
	auto engine = static_cast<Scripter_Lua*>(Engine::get()->getScripter())->getEngine();
	auto typeId = getHash(engine->executeCode<const std::string&>("return " + componentName + ".id"));
	node.getScene()->removeComponent(&node, typeId);
}