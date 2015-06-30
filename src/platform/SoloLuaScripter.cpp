#include "SoloLuaScripter.h"

using namespace solo;


void LuaScripter::execString(const std::string& script)
{
	this->script(script.c_str());
}


void LuaScripter::execFile(const std::string& scriptFileName)
{
	script.runFile(scriptFileName);
}