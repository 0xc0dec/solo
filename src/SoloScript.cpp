#include "SoloScript.h"
#include "platform/SoloLuaScript.h"

using namespace solo;


shared<Script> ScriptFactory::create(Node* node, const std::string& componentObjectName)
{
	return NEW2(LuaScript, node, componentObjectName);
}


Script::Script(Node* node) : ComponentBase(node)
{
}
