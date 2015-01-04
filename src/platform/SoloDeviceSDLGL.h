#ifndef __SOLO_DEVICE_SDLGL_H__
#define __SOLO_DEVICE_SDLGL_H__

#include <SDL.h>
#include "../SoloDevice.h"

namespace solo
{
	class DeviceSDLGL : public Device
	{
	public:
		explicit DeviceSDLGL(const EngineCreationArgs &args);
		~DeviceSDLGL();

		void setWindowTitle(const char *title) override;

		unsigned long getLifetime() const override;
		void update() override;

	private:
		SDL_Window *_window;
		SDL_GLContext _context;

		void _processSystemEvents();
		std::tuple<int, int> _selectContextVersion(int desiredMajorVersion, int desiredMinorVersion);
		std::tuple<SDL_Window*, SDL_GLContext> _tryInitWindowWithContext(bool fake, int ctxMajorVersion, int ctxMinorVersion);
	};
}

#endif