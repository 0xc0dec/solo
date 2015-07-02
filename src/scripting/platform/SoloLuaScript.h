#pragma once

#include "scripting/SoloScript.h"

namespace solo
{
	class Device;

	class LuaScript : public Script
	{
	public:
		virtual ~LuaScript() {}

		virtual void update() override;

		void setUpdateCallback(const std::function<void(float)>& callback);
		void lalala(const std::string& s);

	private:
		friend class ScriptFactory;

		LuaScript(Node* node);

		Device* device = nullptr;
		std::function<void(float)> updateCallback;
	};
}