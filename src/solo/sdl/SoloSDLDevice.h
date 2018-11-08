/* 
 * Copyright (c) Aleksey Fedotov 
 * MIT license 
 */

#pragma once

#include "SoloDevice.h"
#include <SDL.h>

namespace solo
{
    class SDLDevice : public Device
    {
    public:
        virtual ~SDLDevice();

        auto windowTitle() const -> str override final;
        void setWindowTitle(const str &title) override final;

        void setCursorCaptured(bool captured) override final;

        auto lifetime() const -> float override final;

        auto canvasSize() const -> Vector2 override final;
        auto dpiIndependentCanvasSize() const -> Vector2 override final;

    protected:
        SDL_Window *window_ = nullptr;

        bool hasMouseFocus_ = false;
        bool hasKeyboardFocus_ = false;

        explicit SDLDevice(const DeviceSetup &setup);

        void beginUpdate() override final;

    private:
        void prepareKeyboardState();
        void prepareMouseState();
        void readWindowState();
        void readEvents();
        void processKeyboardEvent(const SDL_Event &evt);
        void processMouseEvent(const SDL_Event &evt);
        void processWindowEvent(const SDL_Event &evt);
    };
}
