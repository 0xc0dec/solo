#include <SDL.h>

#include "Engine.h"
#include "Common.h"

using namespace solo;

Engine::Engine()
{
}


Engine::~Engine()
{
}


void Engine::run(const EngineLaunchArgs & launchArgs)
{
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_TIMER) < 0)
		throw Exception();

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_COMPATIBILITY);

	_window.reset(new Window(launchArgs));

	SDL_Event evt;
	auto run = true;
	while (run)
	{
		while (SDL_PollEvent(&evt))
		{
			switch (evt.type)
			{
			case SDL_QUIT:
				run = false;
				break;
			case SDL_KEYUP:
				if (evt.key.keysym.sym == SDLK_ESCAPE)
					run = false;
				break;
			default:
				break;
			}
		}
		
		_window->update();
	}

	_window.reset();

	SDL_Quit();
}
