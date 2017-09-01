/*
    Copyright (c) Aleksey Fedotov
    MIT license
*/

#include "SoloSDLDevice.h"
#include "SoloSDLInputMaps.h"

using namespace solo;

sdl::Device::Device(const DeviceSetup &setup) :
    solo::Device(setup)
{
    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER | SDL_INIT_EVENTS);
}

sdl::Device::~Device()
{
    SDL_Quit();
}

auto sdl::Device::getWindowTitle() const -> std::string
{
    return std::string(SDL_GetWindowTitle(window));
}

void sdl::Device::setWindowTitle(const std::string &title)
{
    SDL_SetWindowTitle(window, title.c_str());
}

void sdl::Device::setCursorCaptured(bool captured)
{
    SDL_SetRelativeMouseMode(captured ? SDL_TRUE : SDL_FALSE);
}

auto sdl::Device::getLifetime() const -> float
{
    return SDL_GetTicks() / 1000.0f;
}

auto sdl::Device::getCanvasSize() const -> Vector2
{
    int32_t width, height;
    SDL_GL_GetDrawableSize(window, &width, &height);
    return {static_cast<float>(width), static_cast<float>(height)};
}

void sdl::Device::beginUpdate()
{
    windowCloseRequested = false;
    quitRequested = false;
    readWindowState();
    prepareMouseState();
    prepareKeyboardState();
    readEvents();
    updateTime();
}

void sdl::Device::prepareKeyboardState()
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

void sdl::Device::prepareMouseState()
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

void sdl::Device::readWindowState()
{
    auto flags = SDL_GetWindowFlags(window);
    hasKeyboardFocus = (flags & SDL_WINDOW_INPUT_FOCUS) != 0;
    hasMouseFocus = (flags & SDL_WINDOW_MOUSE_FOCUS) != 0;
}

void sdl::Device::processKeyboardEvent(const SDL_Event &evt)
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
        default:
            break;
    }
}

void sdl::Device::processMouseEvent(const SDL_Event &evt)
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
        default:
            break;
    }
}

void sdl::Device::processWindowEvent(const SDL_Event &evt)
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

void sdl::Device::readEvents()
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