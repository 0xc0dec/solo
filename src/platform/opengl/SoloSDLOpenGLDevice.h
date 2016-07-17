#pragma once

#include "../sdl/SoloSDLDevice.h"

#ifdef SL_OPENGL_RENDERER

namespace solo
{
    class SDLOpenGLDevice final: public SDLDevice
    {
    public:
        explicit SDLOpenGLDevice(const DeviceCreationArgs& args);
        virtual ~SDLOpenGLDevice();

        void saveScreenshot(const std::string& path) override final;

        auto getCanvasSize() const -> Vector2 override final;

    private:
        void endUpdate() override final;

        SDL_GLContext context = nullptr;
    };
}

#else
#   error OpenGL renderer is not supported on this platform
#endif
