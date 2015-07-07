#pragma once

#include "SoloComponent.h"
#include <chaiscript.hpp>

namespace solo
{
	class Device;

	class ScriptComponent_Chai : public ComponentBase<ScriptComponent_Chai>
	{
	public:
		virtual ~ScriptComponent_Chai() {}

		virtual void update() override;

		static chaiscript::Boxed_Value addComponent(chaiscript::Boxed_Value& boxedNode, const std::string& componentClass);
		static chaiscript::Boxed_Value findComponent(chaiscript::Boxed_Value& boxedNode, const std::string& componentClass);
		static void removeComponent(chaiscript::Boxed_Value& boxedNode, const std::string& componentClass);

	private:
		ScriptComponent_Chai(Node* node, const std::string& componentClass);

		chaiscript::Boxed_Value component;
		std::function<void(chaiscript::Boxed_Value&)> updateFunc;
	};
}