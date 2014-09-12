#include <SDL.h>
#include "SoloEngine.h"
#include "SoloLog.h"

using namespace solo;

Engine::Engine()
	: _stopSignalled(false)
{
}


Engine::~Engine()
{
}


void Engine::_loop()
{
	while (!_stopSignalled)
	{
		_processSystemEvents();
		_window->update();
	}
}


void Engine::_processSystemEvents()
{
	SDL_Event evt;
	while (SDL_PollEvent(&evt))
	{
		switch (evt.type)
		{
		case SDL_QUIT:
			_stopSignalled = true;
			break;
		case SDL_KEYUP:
			if (evt.key.keysym.sym == SDLK_ESCAPE)
				_stopSignalled = true;
			break;
		default:
			break;
		}
	}
}


void Engine::_run(const EngineLaunchArgs & args)
{
	INFO("Starting engine");

	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_TIMER) < 0)
		throw EngineException("Failed to initialize system");

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_COMPATIBILITY);

	_window.reset(new Window(args));
	_loop();
	_window.reset();

	SDL_Quit();
}


void Engine::run(const EngineLaunchArgs & args)
{
	try
	{
		run(args);
	}
	catch (EngineException &e)
	{
		CRITICAL(e.message);
	}
}
