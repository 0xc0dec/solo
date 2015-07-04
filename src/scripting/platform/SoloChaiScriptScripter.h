#pragma once

#include "scripting/SoloScripter.h"

namespace chaiscript
{
	class ChaiScript;
}

namespace solo
{
	class ChaiScriptScripter: public Scripter
	{
	public:
		virtual void execString(const std::string& script) override;
		virtual void execFile(const std::string& scriptFileName) override;

		shared<chaiscript::ChaiScript> getEngine() const;

	private:
		friend class ScripterFactory;

		ChaiScriptScripter();
		void registerScriptApi();

		shared<chaiscript::ChaiScript> engine;
	};
}