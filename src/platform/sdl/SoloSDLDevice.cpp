#include "SoloSDLDevice.h"
#include "SoloSDLInputMaps.h"

using namespace solo;


SDLDevice::SDLDevice(const DeviceSetup& setup):
    Device(setup)
{
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER | SDL_INIT_EVENTS) < 0)
        SL_EXCEPTION(InternalException, "Failed to initialize SDL");
}


SDLDevice::~SDLDevice()
{
    SDL_Quit();
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
        for (const auto& pair : pressedMouseButtons)
            pressedMouseButtons[pair.first] = false;
    }
    else
    {
        for (const auto& pair : pressedMouseButtons)
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