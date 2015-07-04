#pragma once

#include "SoloComponent.h"
#include <chaiscript.hpp>

namespace solo
{
	class Device;

	class ChaiScriptComponent : public ComponentBase<ChaiScriptComponent>
	{
	public:
		virtual ~ChaiScriptComponent() {}

		virtual void update() override;

		static ChaiScriptComponent* addComponent(chaiscript::Boxed_Value&, chaiscript::Boxed_Value&);

	private:
		friend class ScriptFactory_Chai;

		ChaiScriptComponent(Node* node, chaiscript::Boxed_Value obj);

		Device* device = nullptr;
		chaiscript::Boxed_Value component;
		std::function<void(chaiscript::Boxed_Value&, float)> updateFunc;
	};
}