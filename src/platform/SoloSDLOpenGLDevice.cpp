#include <GL/glew.h>
#include "SoloSDLOpenGLDevice.h"
#include "SoloException.h"
#include "SoloLog.h"

using namespace solo;


std::list<std::pair<int, int>> supportedContextVersions =
{
	{ 4, 4 },
	{ 4, 3 },
	{ 4, 2 },
	{ 4, 1 },
	{ 4, 0 },
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


SDLOpenGLDevice::SDLOpenGLDevice(EngineCreationArgs const& args):
	Device(args),
	hasKeyboardFocus(false),
	hasMouseFocus(false),
	window(nullptr),
	context(nullptr)
{
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_TIMER | SDL_INIT_EVENTS) < 0)
		THROW(EngineException, "Failed to initialize system");

	auto ctxVersion = selectContextVersion();
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

	glDepthFunc(GL_LEQUAL);
	glEnable(GL_DEPTH_TEST);

	glFrontFace(GL_CW);
	glEnable(GL_CULL_FACE);

	SDL_GL_SetSwapInterval(1);
}


std::tuple<int, int> SDLOpenGLDevice::selectContextVersion()
{
	for (auto version : supportedContextVersions)
	{
		SDL_GLContext context = nullptr;
		auto windowWithContext = tryCreateWindowWithContext(true, version.first, version.second);
		auto window = std::get<0>(windowWithContext);
		if (window)
		{
			context = std::get<1>(windowWithContext);
			if (context)
			{
				SDL_DestroyWindow(window);
				SDL_GL_DeleteContext(context);
				return version;
			}
		}
	}
	THROW(EngineException, "None of the supported OpenGL versions found");
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


float SDLOpenGLDevice::getLifetime() const
{
	return SDL_GetTicks() / 1000.0f;
}


Vector2 SDLOpenGLDevice::getCanvasSize() const
{
	int width, height;
	SDL_GetWindowSize(window, &width, &height);
	return Vector2(static_cast<float>(width), static_cast<float>(height));
}
