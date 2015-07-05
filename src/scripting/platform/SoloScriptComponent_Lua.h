#pragma once

#include "SoloComponent.h"
#include "SoloNode.h"

namespace solo
{
	class Device;

	class ScriptComponent_Lua : public ComponentBase<ScriptComponent_Lua>
	{
	public:
		void update() override;

		static void addComponent(Node& node, const std::string& componentName);
		static void removeComponent(Node& node, const std::string& componentName);

	private:
		ScriptComponent_Lua(Node* node, const std::string& componentName);

		Device* device;
		std::function<void(float)> updateFunc;
	};
}