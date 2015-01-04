#ifndef __SOLO_I_ENGINE_H__
#define __SOLO_I_ENGINE_H__

#include "SoloBase.h"

namespace solo
{
	struct EngineCreationArgs
	{
		int canvasWidth;
		int canvasHeight;
		int bits;
		int depth;
		bool fullScreen;
		const char *windowTitle;
	};

	class IScene;
	class IDevice;
	class IVideoDriver;
	class IEngineCallback;

	class IEngine
	{
	public:
		virtual ~IEngine() {}

		virtual void run(const EngineCreationArgs &args) = 0;

		virtual void setCallback(IEngineCallback *callback) = 0;

		virtual ptr<IDevice> getDevice() const = 0;
		virtual ptr<IScene> getScene() const = 0;
		virtual ptr<IVideoDriver> getVideoDriver() const = 0;

		virtual float getTimeDelta() const = 0;
	};
}

#endif