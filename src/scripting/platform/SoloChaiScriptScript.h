#pragma once

#include "SoloComponent.h"
#include <chaiscript.hpp>

namespace solo
{
	class Device;

	class ChaiScriptScript : public ComponentBase<ChaiScriptScript>
	{
	public:
		virtual ~ChaiScriptScript() {}

		virtual void update() override;

	private:
		friend class ScriptFactory_Chai;

		ChaiScriptScript(Node* node, chaiscript::Boxed_Value obj);

		Device* device = nullptr;
		chaiscript::Boxed_Value obj;
		std::function<void(chaiscript::Boxed_Value&, float)> updateFunc;
	};

	class ScriptFactory_Chai
	{
	public:
//		friend Script* addComponent(chaiscript::Boxed_Value&, chaiscript::Boxed_Value&);
		static shared<ChaiScriptScript> create(Node* node, chaiscript::Boxed_Value obj);
	};
}