#include "SoloLuaScript.h"
#include "SoloEngine.h"
#include "SoloDevice.h"

using namespace solo;


LuaScript::LuaScript(Node* node): Script(node)
{
	auto engine = Engine::get();
	device = engine->getDevice();
}


void LuaScript::update()
{
	if (updateCallback)
		updateCallback(device->getTimeDelta());
}


void LuaScript::setUpdateCallback(const std::function<void(float)>& callback)
{
	updateCallback = callback;
}


void LuaScript::lalala(const std::string& s)
{
	auto b = s;
}