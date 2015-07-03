#include "scripting/SoloScript.h"
#include "platform/SoloChaiScriptScript.h"

using namespace solo;


shared<Script> ScriptFactory::create(Node* node)
{
	return NEW2(ChaiScriptScript, node);
}


Script::Script(Node* node) : ComponentBase(node)
{
}
