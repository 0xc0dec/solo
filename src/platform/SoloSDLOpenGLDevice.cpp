#include <GL/glew.h>
#include "SoloSDLOpenGLDevice.h"
#include "SoloException.h"
#include "SoloLog.h"

using namespace solo;

#define MAX_GL_CONTEXT_VERSION_MAJOR 4
#define MAX_GL_CONTEXT_VERSION_MINOR 5


std::unordered_map<SDL_Keycode, KeyCode> keymap =
{
	{ SDLK_w, KeyCode::W },
	{ SDLK_s, KeyCode::S },
	{ SDLK_a, KeyCode::A },
	{ SDLK_d, KeyCode::D },
	{ SDLK_LEFT, KeyCode::LeftArrow },
	{ SDLK_RIGHT, KeyCode::RightArrow },
	{ SDLK_UP, KeyCode::UpArrow },
	{ SDLK_DOWN, KeyCode::DownArrow },
	{ SDLK_ESCAPE, KeyCode::Escape },
};

std::unordered_map<Uint8, MouseButton> mouseButtonsMap = 
{
	{ SDL_BUTTON_LEFT, MouseButton::Left },
	{ SDL_BUTTON_MIDDLE, MouseButton::Middle },
	{ SDL_BUTTON_RIGHT, MouseButton::Right },
};


SDLOpenGLDevice::SDLOpenGLDevice(EngineCreationArgs const& args):
	Device(args)
{
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_TIMER | SDL_INIT_EVENTS) < 0)
		THROW(EngineException, "Failed to initialize system");

	auto ctxVersion = selectContextVersion(MAX_GL_CONTEXT_VERSION_MAJOR, MAX_GL_CONTEXT_VERSION_MINOR);
	auto major = std::get<0>(ctxVersion);
	auto minor = std::get<1>(ctxVersion);
	INFO("Using OpenGL context version ", major, ".", minor);

	auto windowWithContext = tryCreateWindowWithContext(false, major, minor);

	window = std::get<0>(windowWithContext);
	if (!window)
		THROW(EngineException, "Failed to create device");

	context = std::get<1>(windowWithContext);
	if (!context)
		THROW(EngineException, "Failed to init OpenGL context");

	glewExperimental = true;
	if (glewInit())
		THROW(EngineException, "Failed to init OpenGL extensions");

	SDL_GL_SetSwapInterval(1);
}


std::tuple<int, int> SDLOpenGLDevice::selectContextVersion(int targetMajorVersion, int targetMinorVersion)
{
	auto maxMinorVersion = targetMinorVersion;
	int minor = maxMinorVersion, major;
	for (major = targetMajorVersion; major >= 2; --major)
	{
		SDL_GLContext context = nullptr;
		for (minor = maxMinorVersion; minor >= 0; --minor)
		{
			auto windowWithContext = tryCreateWindowWithContext(true, major, minor);
			auto window = std::get<0>(windowWithContext);
			if (!window)
				continue;

			context = std::get<1>(windowWithContext);
			if (!context)
				continue;

			SDL_DestroyWindow(window);
			SDL_GL_DeleteContext(context);

			break;
		}
		if (context)
			break;
		// try targetMinorVersion..0 first, then 9..0
		maxMinorVersion = 9;
	}

	return std::make_tuple(major, minor);
}


std::tuple<SDL_Window*, SDL_GLContext> SDLOpenGLDevice::tryCreateWindowWithContext(bool hidden, int ctxMajorVersion, int ctxMinorVersion)
{
	SDL_Window* window;
	SDL_GLContext context;

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, ctxMajorVersion);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, ctxMinorVersion);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, creationArgs.depth);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

	auto flags = static_cast<int>(SDL_WINDOW_OPENGL | SDL_WINDOW_ALLOW_HIGHDPI);
	if (hidden)
		flags |= SDL_WINDOW_HIDDEN;
	window = SDL_CreateWindow(creationArgs.windowTitle, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
		creationArgs.canvasWidth, creationArgs.canvasHeight, flags);
	if (window)
		context = SDL_GL_CreateContext(window);

	return std::make_tuple(window, context);
}


SDLOpenGLDevice::~SDLOpenGLDevice()
{
	SDL_GL_DeleteContext(context);
	SDL_DestroyWindow(window);
	SDL_Quit();
}


void SDLOpenGLDevice::beginUpdate()
{
	readWindowState();
	prepareMouseState();
	prepareKeyboardState();
	readEvents();
}


void SDLOpenGLDevice::endUpdate()
{
	SDL_GL_SwapWindow(window);
}


void SDLOpenGLDevice::setWindowTitle(const char* title)
{
	SDL_SetWindowTitle(window, title);
}


std::string SDLOpenGLDevice::getWindowTitle() const
{
	return std::string(SDL_GetWindowTitle(window));
}


void SDLOpenGLDevice::setCursorCaptured(bool captured)
{
	SDL_SetRelativeMouseMode(captured ? SDL_TRUE : SDL_FALSE);
}


void SDLOpenGLDevice::prepareKeyboardState()
{
	releasedKeys.clear();
	if (!hasKeyboardFocus)
	{
		for (auto pair : pressedKeys)
			releasedKeys.insert(pair.first);
	}
	pressedKeys.clear();
}


void SDLOpenGLDevice::prepareMouseState()
{
	mouseDeltaX = mouseDeltaY = 0;
	releasedMouseButtons.clear();
	if (hasMouseFocus)
	{
		for (auto pair : pressedMouseButtons)
			pressedMouseButtons[pair.first] = false;
	}
	else
	{
		for (auto pair : pressedMouseButtons)
			releasedMouseButtons.insert(pair.first);
		pressedMouseButtons.clear();
	}
}


void SDLOpenGLDevice::readWindowState()
{
	auto flags = SDL_GetWindowFlags(window);
	hasKeyboardFocus = (flags & SDL_WINDOW_INPUT_FOCUS) != 0;
	hasMouseFocus = (flags & SDL_WINDOW_MOUSE_FOCUS) != 0;
}


void SDLOpenGLDevice::processKeyboardEvent(const SDL_Event& evt)
{
	if (!hasKeyboardFocus)
		return;
	switch (evt.type)
	{
		case SDL_KEYDOWN:
		{
			auto code = keymap[evt.key.keysym.sym];
			pressedKeys[code] = evt.key.repeat == 0;
			releasedKeys.erase(code);
			break;
		}
		case SDL_KEYUP:
		{
			auto code = keymap[evt.key.keysym.sym];
			releasedKeys.insert(code);
			pressedKeys.erase(code);
			break;
		}
	}
}


void SDLOpenGLDevice::processMouseEvent(const SDL_Event& evt)
{
	switch (evt.type)
	{
		case SDL_MOUSEMOTION:
			mouseDeltaX = evt.motion.xrel;
			mouseDeltaY = evt.motion.yrel;
			break;
		case SDL_MOUSEBUTTONDOWN:
		{
			auto button = mouseButtonsMap[evt.button.button];
			pressedMouseButtons[button] = true; // pressed for the first time
			releasedMouseButtons.erase(button);
			break;
		}
		case SDL_MOUSEBUTTONUP:
		{
			auto button = mouseButtonsMap[evt.button.button];
			if (pressedMouseButtons.find(button) != pressedMouseButtons.end())
			{
				releasedMouseButtons.insert(button);
				pressedMouseButtons.erase(button);
			}
			break;
		}
	}
}


void SDLOpenGLDevice::processWindowEvent(const SDL_Event& evt)
{
	switch (evt.window.event)
	{
		case SDL_WINDOWEVENT_CLOSE:
			requestShutdown();
			break;
	}
}


void SDLOpenGLDevice::readEvents()
{
	static auto firstTime = true;
	SDL_Event evt;
	while (SDL_PollEvent(&evt))
	{
		switch (evt.type)
		{
			case SDL_QUIT:
				requestShutdown();
				break;
			case SDL_WINDOWEVENT:
				processWindowEvent(evt);
				break;
		}
		if (!firstTime)
		{
			processKeyboardEvent(evt);
			processMouseEvent(evt);
		}
	}
	firstTime = false;
}


unsigned long SDLOpenGLDevice::getLifetime() const
{
	return SDL_GetTicks();
}


Vector2 SDLOpenGLDevice::getCanvasSize() const
{
	int width, height;
	SDL_GetWindowSize(window, &width, &height);
	return Vector2(static_cast<float>(width), static_cast<float>(height));
}
