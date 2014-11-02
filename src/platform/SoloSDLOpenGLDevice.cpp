#include <GL/glew.h>
#include "SoloSDLOpenGLDevice.h"
#include "SoloException.h"
#include "SoloGLSLGPUProgram.h"
#include "../SoloLog.h"

using namespace solo;

#define MAX_GL_CONTEXT_VERSION_MAJOR 4
#define MAX_GL_CONTEXT_VERSION_MINOR 5


SDLOpenGLDevice::SDLOpenGLDevice(EngineCreationArgs const& args)
	: Device(args)
{
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_TIMER) < 0)
		throw EngineException("Failed to initialize system");
	
	auto ctxVersion = _selectContextVersion(MAX_GL_CONTEXT_VERSION_MAJOR, MAX_GL_CONTEXT_VERSION_MINOR);
	auto major = std::get<0>(ctxVersion);
	auto minor = std::get<1>(ctxVersion);
	INFO("Using OpenGL context version " << major << "." << minor);
	
	auto windowWithContext = _tryInitWindowWithContext(false, major, minor);
	
	_window = std::get<0>(windowWithContext);
	if (!_window)
		throw EngineException("Failed to create device");

	_context = std::get<1>(windowWithContext);
	if (!_context)
		throw EngineException("Failed to init OpenGL context");

	glewExperimental = true;
	if (glewInit())
		throw EngineException("Failed to init OpenGL extensions");

	SDL_GL_SetSwapInterval(1);
}


std::tuple<s32, s32> SDLOpenGLDevice::_selectContextVersion(s32 desiredMajorVersion, s32 desiredMinorVersion)
{
	auto maxMinorVersion = desiredMinorVersion;
	s32 minor = maxMinorVersion, major;
	for (major = desiredMajorVersion; major >= 2; --major)
	{
		SDL_GLContext context = nullptr;
		for (minor = maxMinorVersion; minor >= 0; --minor)
		{
			auto windowWithContext = _tryInitWindowWithContext(true, major, minor);
			auto window = std::get<0>(windowWithContext);
			if (!window)
				continue;
			
			context = std::get<1>(windowWithContext);
			if (!context)
				continue;
			
			SDL_DestroyWindow(window);
			
			break;
		}
		if (context)
			break;
		// try desiredMinorVersion..0 first, then 9..0
		maxMinorVersion = 9;
	}
	
	return std::make_tuple(major, minor);
}


std::tuple<SDL_Window*, SDL_GLContext> SDLOpenGLDevice::_tryInitWindowWithContext(bool fake, s32 ctxMajorVersion, s32 ctxMinorVersion)
{
	SDL_Window *window;
	SDL_GLContext context;
	
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, ctxMajorVersion);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, ctxMinorVersion);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, _creationArgs.depth);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	
	auto flags = static_cast<int>(SDL_WINDOW_OPENGL);
	auto title = fake ? "" : _creationArgs.windowTitle;
	auto width = fake ? 100 : _creationArgs.canvasWidth;
	auto height = fake ? 100 : _creationArgs.canvasHeight;
	if (fake)
		flags |= SDL_WINDOW_HIDDEN;
	window = SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, flags);
	if (window)
		context = SDL_GL_CreateContext(window);
	
	return std::make_tuple(window, context);
}


SDLOpenGLDevice::~SDLOpenGLDevice()
{
	SDL_GL_DeleteContext(_context);
	SDL_DestroyWindow(_window);
	SDL_Quit();
}


void SDLOpenGLDevice::update()
{
	_processSystemEvents();
	SDL_GL_SwapWindow(_window);
}


void SDLOpenGLDevice::setWindowTitle(const c8 *title)
{
	SDL_SetWindowTitle(_window, title);
}


sptr<IGPUProgram> SDLOpenGLDevice::createGPUProgram(const str &vsSrc, const str &fsSrc)
{
	auto program = makePtr<GLSLGPUProgram>(vsSrc, fsSrc);
	_gpuPrograms.push_back(program);
	return program;
}


void SDLOpenGLDevice::_processSystemEvents()
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


u64 SDLOpenGLDevice::lifetime()
{
	return SDL_GetTicks();
}