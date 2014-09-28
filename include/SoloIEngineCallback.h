#ifndef __SOLO_I_ENGINE_CALLBACK_H__
#define __SOLO_I_ENGINE_CALLBACK_H__

#include "SoloCommons.h"

namespace solo
{
	class IEngineCallback
	{
	public:
		virtual ~IEngineCallback() {}

		virtual bool onDeviceCloseRequested() = 0;
		virtual void onEngineStarted() = 0;
		virtual void onEngineStopped() = 0;
		virtual void onBeforeFrame(f32 dt) = 0;
	};
}

#endif