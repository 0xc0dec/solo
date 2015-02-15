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
		virtual std::string getWindowTitle() const override;

		virtual void setCursorCaptured(bool captured) override;

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

		void prepareKeyboardState();
		void prepareMouseState();

		void readWindowState();
		void readEvents();
		void processKeyboardEvent(const SDL_Event& evt);
		void processMouseEvent(const SDL_Event& evt);
		void processWindowEvent(const SDL_Event& evt);
		
		bool hasMouseFocus;
		bool hasKeyboardFocus;

		SDL_Window *window;
		SDL_GLContext context;

		std::tuple<int, int> selectContextVersion(int targetMajorVersion, int targetMinorVersion);
		std::tuple<SDL_Window*, SDL_GLContext> tryCreateWindowWithContext(bool fake, int ctxMajorVersion, int ctxMinorVersion);
	};
}
