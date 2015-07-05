//#pragma once
//
//#include "scripting/SoloScripter.h"
//
//namespace chaiscript
//{
//	class ChaiScript;
//}
//
//namespace solo
//{
//	class Scripter_Chai: public Scripter
//	{
//	public:
//		virtual void execString(const std::string& script) override;
//		virtual void execFile(const std::string& scriptFileName) override;
//
//		shared<chaiscript::ChaiScript> getEngine() const;
//
//	private:
//		friend class ScripterFactory;
//
//		Scripter_Chai();
//		void registerScriptApi();
//
//		shared<chaiscript::ChaiScript> engine;
//	};
//}