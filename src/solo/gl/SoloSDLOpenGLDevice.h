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
    namespace gl
    {
        class SDLDevice final : public sdl::Device
        {
        public:
            explicit SDLDevice(const DeviceSetup &setup);
            virtual ~SDLDevice();

            void saveScreenshot(const str &path) override final;

        private:
            SDL_GLContext context = nullptr;

            void endUpdate() override final;

            void cleanup();
        };
    }
}

#endif
