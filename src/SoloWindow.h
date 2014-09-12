#ifndef __SOLO_WINDOW_H__
#define __SOLO_WINDOW_H__

#include <SDL.h>
#include "SoloIEngine.h"
#include "SoloIWindow.h"

namespace solo
{
	class Window: public IWindow
	{
	public:
		Window(EngineLaunchArgs args);
		~Window();

		void update();

	private:
		SDL_Window *_window;
		SDL_GLContext _context;
	};
}

#endif