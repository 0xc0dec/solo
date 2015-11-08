#include "SoloSDLOpenGLDevice.h"
#include "SoloLog.h"
#include <tuple>
#include <GL/glew.h>

using namespace solo;


std::list<std::pair<int, int>> supportedContextVersions =
{
	{ 4, 4 },
	{ 4, 3 },
	{ 4, 2 },
	{ 4, 1 },
	{ 4, 0 },
};


struct WindowWithContextCreationResult
{
	SDL_Window *window;
	SDL_GLContext context;

	WindowWithContextCreationResult(SDL_Window* window, SDL_GLContext context):
		window(window), context(context)
	{
	}

	bool succeeded()
	{
		return window && context;
	}
};


std::unordered_map<SDL_Keycode, KeyCode> keymap =
{
	{ SDLK_q, KeyCode::Q },
	{ SDLK_w, KeyCode::W },
	{ SDLK_e, KeyCode::E },
	{ SDLK_r, KeyCode::R },
	{ SDLK_t, KeyCode::T },
	{ SDLK_y, KeyCode::Y },
	{ SDLK_u, KeyCode::U },
	{ SDLK_i, KeyCode::I },
	{ SDLK_o, KeyCode::O },
	{ SDLK_p, KeyCode::P },

	{ SDLK_a, KeyCode::A },
	{ SDLK_s, KeyCode::S },
	{ SDLK_d, KeyCode::D },
	{ SDLK_f, KeyCode::F },
	{ SDLK_g, KeyCode::G },
	{ SDLK_h, KeyCode::H },
	{ SDLK_j, KeyCode::J },
	{ SDLK_k, KeyCode::K },
	{ SDLK_l, KeyCode::L },

	{ SDLK_z, KeyCode::Z },
	{ SDLK_x, KeyCode::X },
	{ SDLK_c, KeyCode::C },
	{ SDLK_v, KeyCode::V },
	{ SDLK_b, KeyCode::B },
	{ SDLK_n, KeyCode::N },
	{ SDLK_m, KeyCode::M },

	{ SDLK_LEFT, KeyCode::LeftArrow },
	{ SDLK_RIGHT, KeyCode::RightArrow },
	{ SDLK_UP, KeyCode::UpArrow },
	{ SDLK_DOWN, KeyCode::DownArrow },
	{ SDLK_ESCAPE, KeyCode::Escape },
	// TODO
};

std::unordered_map<Uint8, MouseButton> mouseButtonsMap = 
{
	{ SDL_BUTTON_LEFT, MouseButton::Left },
	{ SDL_BUTTON_MIDDLE, MouseButton::Middle },
	{ SDL_BUTTON_RIGHT, MouseButton::Right },
};


WindowWithContextCreationResult tryCreateOpengGLWindow(bool hidden, int ctxMajorVersion, int ctxMinorVersion, DeviceCreationArgs creationArgs)
{
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, ctxMajorVersion);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, ctxMinorVersion);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, creationArgs.depth);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

	auto flags = static_cast<int>(SDL_WINDOW_OPENGL | SDL_WINDOW_ALLOW_HIGHDPI);
	if (hidden)
		flags |= SDL_WINDOW_HIDDEN;
	if (creationArgs.fullScreen)
		flags |= SDL_WINDOW_FULLSCREEN;
	auto window = SDL_CreateWindow(creationArgs.windowTitle.c_str(),
		SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
		creationArgs.canvasWidth, creationArgs.canvasHeight,
		flags);

	if (window)
	{
		auto context = SDL_GL_CreateContext(window);
		return { window, context };
	}

	return { nullptr, nullptr };
}


SDLOpenGLDevice::SDLOpenGLDevice(DeviceCreationArgs const& args):
	Device(args)
{
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_TIMER | SDL_INIT_EVENTS) < 0)
		SL_THROW_FMT(EngineException, "Failed to initialize system");

	auto contextVersion = selectContextVersion();
	auto major = std::get<0>(contextVersion);
	auto minor = std::get<1>(contextVersion);
	SL_LOG_INFO("Using OpenGL context version ", major, ".", minor);

	auto windowWithContext = tryCreateOpengGLWindow(false, major, minor, creationArgs);
	if (!windowWithContext.succeeded())
		SL_THROW_FMT(EngineException, "Failed to create window");

	glewExperimental = true;
	if (glewInit())
	{
		// TODO this code is repeated a bit in different places - refactor it
		SDL_GL_DeleteContext(windowWithContext.context);
		SDL_DestroyWindow(windowWithContext.window);
		SDL_Quit();
		SL_THROW_FMT(EngineException, "Failed to init OpenGL extensions");
	}

	window = windowWithContext.window;
	context = windowWithContext.context;

	glDepthFunc(GL_LEQUAL); // TODO move to Material

	SDL_GL_SetSwapInterval(1);
}


std::tuple<int, int> SDLOpenGLDevice::selectContextVersion()
{
	for (auto version : supportedContextVersions)
	{
		SDL_GLContext context = nullptr;
		auto windowWithContext = tryCreateOpengGLWindow(true, version.first, version.second, creationArgs);
		if (windowWithContext.succeeded())
		{
			SDL_GL_DeleteContext(context);
			SDL_DestroyWindow(window);
			return version;
		}
	}
	SDL_Quit();
	SL_THROW_FMT(EngineException, "No supported OpenGL versions found");
}


SDLOpenGLDevice::~SDLOpenGLDevice()
{
	if (context)
		SDL_GL_DeleteContext(context);
	if (window)
		SDL_DestroyWindow(window);
	SDL_Quit();
}


void SDLOpenGLDevice::beginUpdate()
{
	readWindowState();
	prepareMouseState();
	prepareKeyboardState();
	readEvents();
	updateTime();
}


void SDLOpenGLDevice::endUpdate()
{
	SDL_GL_SwapWindow(window);
}


void SDLOpenGLDevice::setWindowTitle(const std::string &title)
{
	SDL_SetWindowTitle(window, title.c_str());
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
		pressedKeys.clear();
	}
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
		case SDL_KEYUP:
		case SDL_KEYDOWN:
		{
			auto it = keymap.find(evt.key.keysym.sym);
			if (it == keymap.end())
				break;
			auto code = it->second;
			if (evt.type == SDL_KEYUP)
			{
				releasedKeys.insert(code);
				pressedKeys.erase(code);
			}
			else
			{
				pressedKeys[code] = evt.key.repeat == 0;
				releasedKeys.erase(code);
			}
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


float SDLOpenGLDevice::getLifetime() const
{
	return SDL_GetTicks() / 1000.0f;
}


Vector2 SDLOpenGLDevice::getCanvasSize() const
{
	int width, height;
	SDL_GL_GetDrawableSize(window, &width, &height);
	return Vector2(static_cast<float>(width), static_cast<float>(height));
}
