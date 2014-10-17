#ifndef __SOLO_DEVICE_SDL_H__
#define __SOLO_DEVICE_SDL_H__

#include <SDL.h>
#include <tuple>
#include "SoloDevice.h"

namespace solo
{
	class DeviceSDL : public Device
	{
	public:
		explicit DeviceSDL(EngineCreationArgs const& args);
		~DeviceSDL();

		void setWindowTitle(char const* caption) override;

		u64 lifetime() override;
		void update() override;

	private:
		SDL_Window *_window;
		SDL_GLContext _context;

		void _processSystemEvents();
		void _selectContextVersion(s32 &desiredMajorVersion, s32 &desiredMinorVersion);
		std::tuple<SDL_Window*, SDL_GLContext> _tryInitWindowWithContext(bool fake, s32 ctxMajorVersion, s32 ctxMinorVersion);
	};
}

#endif