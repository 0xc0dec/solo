#ifndef __SOLO_I_ENGINE_CALLBACK_H__
#define __SOLO_I_ENGINE_CALLBACK_H__

#include "SoloCommons.h"

namespace solo
{
	class IEngineCallback
	{
	public:
		virtual ~IEngineCallback() {}

		virtual bool processDeviceCloseRequestedEvent() = 0;
		virtual void processEngineStartedEvent() = 0;
		virtual void processEngineStoppedEvent() = 0;
		virtual void processBeforeFrameEvent() = 0;
	};
}

#endif