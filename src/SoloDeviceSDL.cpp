#include "SoloDeviceSDL.h"
#include "SoloCommonsInternal.h"
#include "SoloException.h"

using namespace solo;


DeviceSDL::DeviceSDL(EngineLaunchArgs const& args)
	: Device(args)
{
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_TIMER) < 0)
		throw EngineException("Failed to initialize system");

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_COMPATIBILITY);

	_window = SDL_CreateWindow(args.windowTitle, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
		args.canvasWidth, args.canvasHeight, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);
	if (!_window)
		throw EngineException("Failed to create device");

	_context = SDL_GL_CreateContext(_window);

	if (!_context)
		throw EngineException("Failed to init GL context");

	SDL_GL_SetSwapInterval(1);
}


DeviceSDL::~DeviceSDL()
{
	SDL_GL_DeleteContext(_context);
	SDL_DestroyWindow(_window);
	SDL_Quit();
}


void DeviceSDL::update()
{
	_processSystemEvents();
	SDL_GL_SwapWindow(_window);
}


void DeviceSDL::setWindowTitle(char const* caption)
{
	SDL_SetWindowTitle(_window, caption);
}


void DeviceSDL::_processSystemEvents()
{
	SDL_Event evt;
	while (SDL_PollEvent(&evt))
	{
		switch (evt.type)
		{
		case SDL_QUIT:
			_closeRequested = true;
			break;
		case SDL_KEYUP:
			if (evt.key.keysym.sym == SDLK_ESCAPE)
				_closeRequested = true;
			break;
		default:
			break;
		}
	}
}


u64 DeviceSDL::lifetime()
{
	return SDL_GetTicks();
}