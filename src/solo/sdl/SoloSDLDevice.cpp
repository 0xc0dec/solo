/*
    Copyright (c) Aleksey Fedotov
    MIT license
*/

#include "SoloSDLDevice.h"
#include "SoloSDLInputMaps.h"

using namespace solo;

sdl::SDLDevice::SDLDevice(const DeviceSetup &setup) :
    Device(setup)
{
    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER | SDL_INIT_EVENTS);
}

sdl::SDLDevice::~SDLDevice()
{
    SDL_Quit();
}

auto sdl::SDLDevice::getWindowTitle() const -> str
{
    return str(SDL_GetWindowTitle(window));
}

void sdl::SDLDevice::setWindowTitle(const str &title)
{
    SDL_SetWindowTitle(window, title.c_str());
}

void sdl::SDLDevice::setCursorCaptured(bool captured)
{
    SDL_SetRelativeMouseMode(captured ? SDL_TRUE : SDL_FALSE);
}

auto sdl::SDLDevice::getLifetime() const -> float
{
    return SDL_GetTicks() / 1000.0f;
}

auto sdl::SDLDevice::getCanvasSize() const -> Vector2
{
    s32 width, height;
    SDL_GL_GetDrawableSize(window, &width, &height);
    return {static_cast<float>(width), static_cast<float>(height)};
}

void sdl::SDLDevice::beginUpdate()
{
    windowCloseRequested = false;
    quitRequested = false;
    readWindowState();
    prepareMouseState();
    prepareKeyboardState();
    readEvents();
    updateTime();
}

void sdl::SDLDevice::prepareKeyboardState()
{
    releasedKeys.clear();
    if (hasKeyboardFocus)
    {
        for (auto &pair : pressedKeys)
            pair.second = false; // not "pressed for the first time" anymore
    }
    else
    {
        for (const auto &pair : pressedKeys)
            releasedKeys.insert(pair.first);
        pressedKeys.clear();
    }
}

void sdl::SDLDevice::prepareMouseState()
{
    mouseDeltaX = mouseDeltaY = 0;
    releasedMouseButtons.clear();
    if (hasMouseFocus)
    {
        for (const auto &pair : pressedMouseButtons)
            pressedMouseButtons[pair.first] = false;
    }
    else
    {
        for (const auto &pair : pressedMouseButtons)
            releasedMouseButtons.insert(pair.first);
        pressedMouseButtons.clear();
    }
}

void sdl::SDLDevice::readWindowState()
{
    const auto flags = SDL_GetWindowFlags(window);
    hasKeyboardFocus = (flags & SDL_WINDOW_INPUT_FOCUS) != 0;
    hasMouseFocus = (flags & SDL_WINDOW_MOUSE_FOCUS) != 0;
}

void sdl::SDLDevice::processKeyboardEvent(const SDL_Event &evt)
{
    if (!hasKeyboardFocus)
        return;

    switch (evt.type)
    {
        case SDL_KEYUP:
        case SDL_KEYDOWN:
        {
            auto it = SDLKeyMap.find(evt.key.keysym.sym);
            if (it == SDLKeyMap.end())
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
        default:
            break;
    }
}

void sdl::SDLDevice::processMouseEvent(const SDL_Event &evt)
{
    if (!hasMouseFocus)
        return;

    switch (evt.type)
    {
        case SDL_MOUSEMOTION:
            mouseDeltaX += evt.motion.xrel;
            mouseDeltaY += evt.motion.yrel;
            break;
        case SDL_MOUSEBUTTONDOWN:
        {
            const auto button = SDLMouseButtonsMap.at(evt.button.button);
            pressedMouseButtons[button] = true; // pressed for the first time
            releasedMouseButtons.erase(button);
            break;
        }
        case SDL_MOUSEBUTTONUP:
        {
            const auto button = SDLMouseButtonsMap.at(evt.button.button);
            if (pressedMouseButtons.find(button) != pressedMouseButtons.end())
            {
                releasedMouseButtons.insert(button);
                pressedMouseButtons.erase(button);
            }
            break;
        }
        default:
            break;
    }
}

void sdl::SDLDevice::processWindowEvent(const SDL_Event &evt)
{
    switch (evt.window.event)
    {
        case SDL_WINDOWEVENT_CLOSE:
            windowCloseRequested = true;
            break;
        default:
            break;
    }
}

void sdl::SDLDevice::readEvents()
{
    static auto firstTime = true;
    SDL_Event evt;
    while (SDL_PollEvent(&evt))
    {
        switch (evt.type)
        {
            case SDL_QUIT:
                quitRequested = true;
                break;
            case SDL_WINDOWEVENT:
                processWindowEvent(evt);
                break;
            default:
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
