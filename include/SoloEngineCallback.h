#pragma once

namespace solo
{
	class EngineCallback
	{
	public:
		virtual ~EngineCallback() {}

		virtual bool onDeviceCloseRequested() = 0;
		virtual void onEngineStarted() = 0;
		virtual void onEngineStopped() = 0;
	};
}
