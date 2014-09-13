#ifndef __SOLO_I_ENGINE_H__
#define __SOLO_I_ENGINE_H__

#include "SoloCommons.h"

namespace solo
{
	struct EngineLaunchArgs
	{
		int canvasWidth;
		int canvasHeight;
		int bits;
		int depth;
		bool fullScreen;
		const char *windowTitle;
	};

	class IWindow;
	class IScene;

	class IEngine
	{
	public:
		virtual ~IEngine() {}

		virtual void run(const EngineLaunchArgs &args) = 0;

		virtual sptr<IWindow> getWindow() const = 0;
		virtual sptr<IScene> getScene() const = 0;
	};
}

#endif