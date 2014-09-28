#ifndef __SOLO_I_ENGINE_H__
#define __SOLO_I_ENGINE_H__

#include "SoloCommons.h"

namespace solo
{
	struct EngineLaunchArgs
	{
		s32 canvasWidth;
		s32 canvasHeight;
		s32 bits;
		s32 depth;
		bool fullScreen;
		const c8 *windowTitle;
	};

	class IScene;
	class IDevice;
	class IEngineCallback;

	class IEngine
	{
	public:
		virtual ~IEngine() {}

		virtual void run(const EngineLaunchArgs &args) = 0;

		virtual void setCallback(IEngineCallback *callback) = 0;

		virtual sptr<IDevice> device() const = 0;
		virtual sptr<IScene> scene() const = 0;
	};
}

#endif