/*
    Copyright (c) Aleksey Fedotov

    This software is provided 'as-is', without any express or implied
    warranty. In no event will the authors be held liable for any damages
    arising from the use of this software.

    Permission is granted to anyone to use this software for any purpose,
    including commercial applications, and to alter it and redistribute it
    freely, subject to the following restrictions:

    1. The origin of this software must not be misrepresented; you must not
        claim that you wrote the original software. If you use this software
        in a product, an acknowledgment in the product documentation would be
        appreciated but is not required.
    2. Altered source versions must be plainly marked as such, and must not be
        misrepresented as being the original software.
    3. This notice may not be removed or altered from any source distribution.
*/

#pragma once

#include "SoloDevice.h"
#include <SDL.h>


namespace solo
{
    class SDLDevice: public Device
    {
    public:
        virtual ~SDLDevice();

        auto getWindowTitle() const->std::string override final;
        void setWindowTitle(const std::string &title) override final;

        void setCursorCaptured(bool captured) override final;

        auto getLifetime() const -> float override final;

    protected:
        explicit SDLDevice(const DeviceSetup &setup);

        void beginUpdate() override final;

        SDL_Window *window = nullptr;

        bool hasMouseFocus = false;
        bool hasKeyboardFocus = false;

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