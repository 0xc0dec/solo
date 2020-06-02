/* 
 * Copyright (c) Aleksey Fedotov 
 * MIT license 
 */

#pragma once

#include "SoloCommon.h"

#ifdef SL_OPENGL_RENDERER

#include "../sdl/SoloSDLDevice.h"

namespace solo
{
    class OpenGLSDLDevice final : public SDLDevice
    {
    public:
        explicit OpenGLSDLDevice(const DeviceSetup &setup);
        virtual ~OpenGLSDLDevice();

    	auto context() const -> const SDL_GLContext& { return context_; }

    private:
        SDL_GLContext context_ = nullptr;

        void endUpdate() override;

        void cleanup();
    };
}

#endif
