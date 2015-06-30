#pragma once

#include "SoloScripter.h"
#include <inclua.h>

namespace solo
{
	class LuaScripter: public Scripter
	{
	public:
		virtual void execString(const std::string& script) override;
		virtual void execFile(const std::string& scriptFileName) override;

	private:
		inclua::Script script{ true };
	};
}