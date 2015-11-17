#include "SoloScriptManager.h"
#include "platform/scripting/lua/SoloLuaScriptManager.h"

using namespace solo;


shared<ScriptManager> ScriptManager::create()
{
	return SL_NEW2(LuaScriptManager);
}