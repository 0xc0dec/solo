/* 
 * Copyright (c) Aleksey Fedotov 
 * MIT license 
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

auto SDLDevice::windowTitle() const -> str
{
    return str(SDL_GetWindowTitle(window_));
}

void SDLDevice::setWindowTitle(const str &title)
{
    SDL_SetWindowTitle(window_, title.c_str());
}

void SDLDevice::setCursorCaptured(bool captured)
{
    SDL_SetRelativeMouseMode(captured ? SDL_TRUE : SDL_FALSE);
}

auto SDLDevice::lifetime() const -> float
{
    return SDL_GetTicks() / 1000.0f;
}

auto SDLDevice::canvasSize() const -> Vector2
{
    s32 width, height;
    SDL_GL_GetDrawableSize(window_, &width, &height);
    return {static_cast<float>(width), static_cast<float>(height)};
}

auto SDLDevice::dpiIndependentCanvasSize() const -> Vector2
{
    s32 width, height;
    SDL_GetWindowSize(window_, &width, &height);
    return {static_cast<float>(width), static_cast<float>(height)};
}

void SDLDevice::beginUpdate()
{
    windowCloseRequested_ = false;
    quitRequested_ = false;
    readWindowState();
    prepareMouseState();
    prepareKeyboardState();
    readEvents();
    updateTime();
}

void SDLDevice::prepareKeyboardState()
{
    releasedKeys_.clear();
    if (hasKeyboardFocus_)
    {
        for (auto &pair : pressedKeys_)
            pair.second = false; // not "pressed for the first time" anymore
    }
    else
    {
        for (const auto &pair : pressedKeys_)
            releasedKeys_.insert(pair.first);
        pressedKeys_.clear();
    }
}

void SDLDevice::prepareMouseState()
{
    int x, y;
    SDL_GetMouseState(&x, &y);
    mousePos_.x() = x;
    mousePos_.y() = y;
    
    mouseDelta_.x() = mouseDelta_.y() = 0;
    releasedMouseButtons_.clear();
    if (hasMouseFocus_)
    {
        for (const auto &pair : pressedMouseButtons_)
            pressedMouseButtons_[pair.first] = false;
    }
    else
    {
        for (const auto &pair : pressedMouseButtons_)
            releasedMouseButtons_.insert(pair.first);
        pressedMouseButtons_.clear();
    }
}

void SDLDevice::readWindowState()
{
    const auto flags = SDL_GetWindowFlags(window_);
    hasKeyboardFocus_ = (flags & SDL_WINDOW_INPUT_FOCUS) != 0;
    hasMouseFocus_ = (flags & SDL_WINDOW_MOUSE_FOCUS) != 0;
}

void SDLDevice::processKeyboardEvent(const SDL_Event &evt)
{
    if (!hasKeyboardFocus_)
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
                releasedKeys_.insert(code);
                pressedKeys_.erase(code);
            }
            else
            {
                pressedKeys_[code] = pressedKeys_.find(code) == pressedKeys_.end();
                releasedKeys_.erase(code);
            }
            break;
        }
        default:
            break;
    }
}

void SDLDevice::processMouseEvent(const SDL_Event &evt)
{
    if (!hasMouseFocus_)
        return;

    switch (evt.type)
    {
        case SDL_MOUSEMOTION:
        {
            mouseDelta_.x() += evt.motion.xrel;
            mouseDelta_.y() += evt.motion.yrel;
            mousePos_.x() = evt.motion.x;
            mousePos_.y() = evt.motion.y;
            break;
        }
        case SDL_MOUSEBUTTONDOWN:
        {
            const auto button = SDLMouseButtonsMap.at(evt.button.button);
            pressedMouseButtons_[button] = true; // pressed for the first time
            releasedMouseButtons_.erase(button);
            break;
        }
        case SDL_MOUSEBUTTONUP:
        {
            const auto button = SDLMouseButtonsMap.at(evt.button.button);
            if (pressedMouseButtons_.find(button) != pressedMouseButtons_.end())
            {
                releasedMouseButtons_.insert(button);
                pressedMouseButtons_.erase(button);
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
            windowCloseRequested_ = true;
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
                quitRequested_ = true;
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
