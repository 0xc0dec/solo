#include "Window.h"
#include "Common.h"

using namespace solo;


Window::Window(EngineLaunchArgs args)
{
	_window = SDL_CreateWindow(args.windowTitle,
		SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
		args.canvasWidth, args.canvasHeight,
		SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);
	if (!_window)
		throw Exception();

	_context = SDL_GL_CreateContext(_window);
	if (!_context)
		throw Exception();

	SDL_GL_SetSwapInterval(1);
}


Window::~Window()
{
	SDL_GL_DeleteContext(_context);
	SDL_DestroyWindow(_window);
}


void Window::update()
{
	SDL_GL_SwapWindow(_window);
}