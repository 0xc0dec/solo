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

		void setWindowTitle(const c8 *title) override;

		sptr<IGPUProgram> createGPUProgram(const str &vsSrc, const str &fsSrc) override;

		u64 lifetime() const override;
		void update() override;

	private:
		SDL_Window *_window;
		SDL_GLContext _context;

		void _processSystemEvents();
		std::tuple<s32, s32> _selectContextVersion(s32 desiredMajorVersion, s32 desiredMinorVersion);
		std::tuple<SDL_Window*, SDL_GLContext> _tryInitWindowWithContext(bool fake, s32 ctxMajorVersion, s32 ctxMinorVersion);
	};
}

#endif