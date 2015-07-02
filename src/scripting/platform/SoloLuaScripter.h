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

		chaiscript::ChaiScript* getScriptEngine() const;

		template <typename T> T eval(const std::string& code);

	private:
		friend class ScripterFactory;

		LuaScripter();
		void registerScriptApi();

		shared<chaiscript::ChaiScript> engine;
	};
}