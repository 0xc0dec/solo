#include "scripting/SoloScript.h"
#include "platform/SoloLuaScript.h"

using namespace solo;


shared<Script> ScriptFactory::create(Node* node, const std::string& callbackObjectName)
{
	return NEW2(LuaScript, node, callbackObjectName);
}


Script::Script(Node* node) : ComponentBase(node)
{
}
