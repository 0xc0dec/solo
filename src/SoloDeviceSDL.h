#ifndef __SOLO_DEVICE_SDL_H__
#define __SOLO_DEVICE_SDL_H__

#include <SDL.h>
#include "SoloDevice.h"

namespace solo
{
	class DeviceSDL : public Device
	{
	public:
		explicit DeviceSDL(EngineLaunchArgs const& args);
		~DeviceSDL();

		void setWindowTitle(char const* caption) override;

		unsigned long getLifetime() override;
		void update() override;

	private:
		SDL_Window *_window;
		SDL_GLContext _context;

		void _processSystemEvents();
	};
}

#endif