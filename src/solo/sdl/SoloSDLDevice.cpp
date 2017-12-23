/*
    Copyright (c) Aleksey Fedotov
    MIT license
*/

#include "SoloSDLDevice.h"
#include "SoloSDLInputMaps.h"

using namespace solo;

SDLDevice::SDLDevice(const DeviceSetup &setup) :
    Device(setup)
{
    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER | SDL_INIT_EVENTS);
}

SDLDevice::~SDLDevice()
{
    SDL_Quit();
}

auto SDLDevice::getWindowTitle() const -> str
{
    return str(SDL_GetWindowTitle(window));
}

void SDLDevice::setWindowTitle(const str &title)
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

auto SDLDevice::getCanvasSize() const -> Vector2
{
    s32 width, height;
    SDL_GL_GetDrawableSize(window, &width, &height);
    return {static_cast<float>(width), static_cast<float>(height)};
}

auto SDLDevice::getDpiIndependentCanvasSize() const -> Vector2
{
    s32 width, height;
    SDL_GetWindowSize(window, &width, &height);
    return {static_cast<float>(width), static_cast<float>(height)};
}

void SDLDevice::beginUpdate()
{
    windowCloseRequested = false;
    quitRequested = false;
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

void SDLDevice::prepareMouseState()
{
    int x, y;
    SDL_GetMouseState(&x, &y);
    mousePos.x = x;
    mousePos.y = y;
    
    mouseDelta.x = mouseDelta.y = 0;
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

void SDLDevice::readWindowState()
{
    const auto flags = SDL_GetWindowFlags(window);
    hasKeyboardFocus = (flags & SDL_WINDOW_INPUT_FOCUS) != 0;
    hasMouseFocus = (flags & SDL_WINDOW_MOUSE_FOCUS) != 0;
}

void SDLDevice::processKeyboardEvent(const SDL_Event &evt)
{
    if (!hasKeyboardFocus)
        return;

    switch (evt.type)
    {
        case SDL_KEYUP:
        case SDL_KEYDOWN:
        {
            const auto it = SDLKeyMap.find(evt.key.keysym.sym);
            if (it == SDLKeyMap.end())
                break;
            const auto code = it->second;
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

void SDLDevice::processMouseEvent(const SDL_Event &evt)
{
    if (!hasMouseFocus)
        return;

    switch (evt.type)
    {
        case SDL_MOUSEMOTION:
        {
            mouseDelta.x += evt.motion.xrel;
            mouseDelta.y += evt.motion.yrel;
            mousePos.x = evt.motion.x;
            mousePos.y = evt.motion.y;
            break;
        }
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

void SDLDevice::processWindowEvent(const SDL_Event &evt)
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

void SDLDevice::readEvents()
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
