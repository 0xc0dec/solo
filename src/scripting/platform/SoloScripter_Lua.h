#pragma once

#include "scripting/SoloScripter.h"
#include <LuaContext.hpp>

namespace solo
{
	class Scripter_Lua: public Scripter
	{
	public:
		virtual void execString(const std::string& script) override;
		virtual void execFile(const std::string& scriptFilePath) override;

		LuaContext* getEngine() const;

	private:
		friend class ScripterFactory;

		Scripter_Lua();
		void registerScriptApi();

		shared<LuaContext> engine;
	};
}