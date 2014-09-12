#ifndef __SOLO_ENGINE_H__
#define __SOLO_ENGINE_H__

#include "Common.h"
#include "Window.h"
#include "IEngine.h"

namespace solo
{
	class Engine: public IEngine
	{
	public:
		Engine();
		~Engine();

		static Engine *getEngine()
		{
			static Engine instance;
			return &instance;
		}

		void run(const EngineLaunchArgs &launchArgs) override;

		sptr<IWindow> getWindow() const override
		{
			return _window;
		}

	private:
		sptr<Window> _window;
	};
}

#endif