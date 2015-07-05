#pragma once

#include "SoloComponent.h"
#include "SoloNode.h"
#include <boost/variant.hpp>

namespace solo
{
	class Device;
	class Transform;
	class Camera;

	class ScriptComponent_Lua : public ComponentBase<ScriptComponent_Lua>
	{
	public:
		void update() override;

		static void addComponent(Node& node, const std::string& expression);
		static void removeComponent(Node& node, const std::string& expression);
		static boost::variant<Transform*, Camera*> findComponent(Node& node, const std::string& name);

	private:
		ScriptComponent_Lua(Node* node, const std::string& componentName);

		Device* device;
		std::function<void(float)> updateFunc;
	};
}