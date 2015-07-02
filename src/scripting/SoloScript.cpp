#include "scripting/SoloScript.h"
#include "platform/SoloLuaScript.h"

using namespace solo;


shared<Script> ScriptFactory::create(Node* node)
{
	return NEW2(LuaScript, node);
}


Script::Script(Node* node) : ComponentBase(node)
{
}
