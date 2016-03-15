#include "SoloScriptManager.h"
#include "scripting/lua/SoloLuaScriptManager.h"

using namespace solo;


shared<ScriptManager> ScriptManager::create()
{
    return SL_NEW_SHARED(LuaScriptManager);
}