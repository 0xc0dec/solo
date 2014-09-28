#ifndef __SOLO_EMPTY_ENGINE_CALLBACK_H__
#define __SOLO_EMPTY_ENGINE_CALLBACK_H__

#include "SoloIEngineCallback.h"


namespace solo
{
	class EmptyEngineCallback: public IEngineCallback
	{
	public:
		bool onDeviceCloseRequested() override { return true; }
		void onEngineStarted() override {}
		void onEngineStopped() override {}
		void onBeforeFrame(float dt) override {}
	};
}

#endif