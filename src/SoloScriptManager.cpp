#include "SoloScriptManager.h"
#include "scripting/lua/SoloLuaScriptManager.h"

using namespace solo;


shared<ScriptManager> ScriptManager::create(Device* device)
{
    // TODO maybe other implementations some day
    return SL_NEW_SHARED(LuaScriptManager, device);
}


ScriptManager::ScriptManager(Device* device):
    device(device)
{
}
