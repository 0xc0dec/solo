#pragma once

#include "scripting/SoloScripter.h"

namespace chaiscript
{
	class ChaiScript;
}

namespace solo
{
	class LuaScripter: public Scripter
	{
	public:
		virtual void execString(const std::string& script) override;
		virtual void execFile(const std::string& scriptFileName) override;

	private:
		friend class ScripterFactory;

		LuaScripter();
		void registerScriptApi();

		chaiscript::ChaiScript *engine;
	};
}