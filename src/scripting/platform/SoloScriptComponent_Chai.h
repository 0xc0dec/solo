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

		static chaiscript::Boxed_Value& addComponent(chaiscript::Boxed_Value& node, const std::string& componentClass);

	private:
		ChaiScriptComponent(Node* node, const std::string& componentClass);

		Device* device = nullptr;
		chaiscript::Boxed_Value component;
		std::function<void(chaiscript::Boxed_Value&, float)> updateFunc;
	};
}