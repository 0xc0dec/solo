#ifndef __SOLO_I_ENGINE_H__
#define __SOLO_I_ENGINE_H__

#include "SoloCommon.h"

namespace solo
{
	struct EngineCreationArgs
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

		virtual void run(const EngineCreationArgs &args) = 0;

		virtual void setCallback(IEngineCallback *callback) = 0;

		virtual ptr<IDevice> device() const = 0;
		virtual ptr<IScene> scene() const = 0;

		virtual f32 deltaTime() const = 0;
	};
}

#endif