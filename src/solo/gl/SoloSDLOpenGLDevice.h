/*
    Copyright (c) Aleksey Fedotov
    MIT license
*/

#pragma once

#include "SoloCommon.h"

#ifdef SL_OPENGL_RENDERER

#include "../sdl/SoloSDLDevice.h"

namespace solo
{
    class SDLOpenGLDevice final : public SDLDevice
    {
    public:
        explicit SDLOpenGLDevice(const DeviceSetup &setup);
        virtual ~SDLOpenGLDevice();

        void saveScreenshot(const str &path) override final;

    private:
        SDL_GLContext context = nullptr;

        void endUpdate() override final;

        void cleanup();
    };
}

#endif
