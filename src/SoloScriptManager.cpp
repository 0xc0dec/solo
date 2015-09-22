#include "SoloScriptManager.h"
#include "platform/scripting/SoloLuaScriptManager.h"

using namespace solo;


shared<ScriptManager> ScriptManagerFactory::create(Engine *engine)
{
	return NEW2(LuaScriptManager, engine);
}