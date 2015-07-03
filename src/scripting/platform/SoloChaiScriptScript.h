#pragma once

#include "scripting/SoloScript.h"

namespace solo
{
	class Device;

	class ChaiScriptScript : public Script
	{
	public:
		virtual ~ChaiScriptScript() {}

		virtual void update() override;

		void setUpdateCallback(const std::function<void(float)>& callback);

	private:
		friend class ScriptFactory;

		ChaiScriptScript(Node* node);

		Device* device = nullptr;
		std::function<void(float)> updateCallback;
	};
}