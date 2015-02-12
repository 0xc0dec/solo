#pragma once

#include <SDL.h>
#include "SoloDevice.h"

namespace solo
{
	class SDLOpenGLDevice : public Device
	{
	public:
		virtual ~SDLOpenGLDevice() override;

		virtual void setWindowTitle(const char *title) override;

		virtual unsigned long getLifetime() const override;

		virtual Vector2 getCanvasSize() const override;

		virtual void beginUpdate() override;
		virtual void endUpdate() override;

	private:
		friend class DeviceFactory;

		SDLOpenGLDevice(const EngineCreationArgs &args);
		SDLOpenGLDevice(const SDLOpenGLDevice& other) = delete;
		SDLOpenGLDevice(SDLOpenGLDevice&& other) = delete;
		SDLOpenGLDevice& operator=(const SDLOpenGLDevice& other) = delete;
		SDLOpenGLDevice& operator=(SDLOpenGLDevice&& other) = delete;

		SDL_Window *window;
		SDL_GLContext context;

		void processSystemEvents();
		std::tuple<int, int> selectContextVersion(int desiredMajorVersion, int desiredMinorVersion);
		std::tuple<SDL_Window*, SDL_GLContext> tryCreateWindowWithContext(bool fake, int ctxMajorVersion, int ctxMinorVersion);
	};
}
