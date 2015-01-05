#pragma once

namespace solo
{
	class IEngineCallback
	{
	public:
		virtual ~IEngineCallback() {}

		virtual bool onDeviceCloseRequested() = 0;
		virtual void onEngineStarted() = 0;
		virtual void onEngineStopped() = 0;
	};
}
