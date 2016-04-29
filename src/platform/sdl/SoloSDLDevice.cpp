#include "SoloSDLDevice.h"

using namespace solo;


static std::unordered_map<SDL_Keycode, KeyCode> keymap =
{
    { SDLK_0, KeyCode::Digit0 },
    { SDLK_1, KeyCode::Digit1 },
    { SDLK_2, KeyCode::Digit2 },
    { SDLK_3, KeyCode::Digit3 },
    { SDLK_4, KeyCode::Digit4 },
    { SDLK_5, KeyCode::Digit5 },
    { SDLK_6, KeyCode::Digit6 },
    { SDLK_7, KeyCode::Digit7 },
    { SDLK_8, KeyCode::Digit8 },
    { SDLK_9, KeyCode::Digit9 },

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

    { SDLK_SPACE, KeyCode::Space },
    { SDLK_BACKSPACE, KeyCode::Backspace }
    // TODO
};


static std::unordered_map<Uint8, MouseButton> mouseButtonsMap =
{
    { SDL_BUTTON_LEFT, MouseButton::Left },
    { SDL_BUTTON_MIDDLE, MouseButton::Middle },
    { SDL_BUTTON_RIGHT, MouseButton::Right },
};


SDLDevice::SDLDevice(const DeviceCreationArgs& args):
    Device(args)
{
}


auto SDLDevice::getWindowTitle() const -> std::string
{
    return std::string(SDL_GetWindowTitle(window));
}


void SDLDevice::setWindowTitle(const std::string& title)
{
    SDL_SetWindowTitle(window, title.c_str());
}


void SDLDevice::setCursorCaptured(bool captured)
{
    SDL_SetRelativeMouseMode(captured ? SDL_TRUE : SDL_FALSE);
}


auto SDLDevice::getLifetime() const -> float
{
    return SDL_GetTicks() / 1000.0f;
}


void SDLDevice::beginUpdate()
{
    readWindowState();
    prepareMouseState();
    prepareKeyboardState();
    readEvents();
    updateTime();
}


void SDLDevice::prepareKeyboardState()
{
    releasedKeys.clear();
    if (hasKeyboardFocus)
    {
        for (auto& pair : pressedKeys)
            pair.second = false; // not "pressed for the first time" anymore
    }
    else
    {
        for (const auto& pair : pressedKeys)
            releasedKeys.insert(pair.first);
        pressedKeys.clear();
    }
}


void SDLDevice::prepareMouseState()
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


void SDLDevice::readWindowState()
{
    auto flags = SDL_GetWindowFlags(window);
    hasKeyboardFocus = (flags & SDL_WINDOW_INPUT_FOCUS) != 0;
    hasMouseFocus = (flags & SDL_WINDOW_MOUSE_FOCUS) != 0;
}


void SDLDevice::processKeyboardEvent(const SDL_Event& evt)
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
            pressedKeys[code] = pressedKeys.find(code) == pressedKeys.end(); // first time?
            releasedKeys.erase(code);
        }
        break;
    }
    }
}


void SDLDevice::processMouseEvent(const SDL_Event& evt)
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


void SDLDevice::processWindowEvent(const SDL_Event& evt)
{
    switch (evt.window.event)
    {
    case SDL_WINDOWEVENT_CLOSE:
        stopRunning();
        break;
    }
}


void SDLDevice::readEvents()
{
    static auto firstTime = true;
    SDL_Event evt;
    while (SDL_PollEvent(&evt))
    {
        switch (evt.type)
        {
        case SDL_QUIT:
            stopRunning();
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