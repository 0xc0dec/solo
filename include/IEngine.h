#ifndef __SOLO_I_ENGINE_H__
#define __SOLO_I_ENGINE_H__

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

	class IEngine
	{
	public:
		virtual ~IEngine() {}

		virtual void run(const EngineLaunchArgs &launchArgs) = 0;

		virtual sptr<IWindow> getWindow() const = 0;
	};
}

#endif