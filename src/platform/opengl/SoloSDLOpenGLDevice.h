#pragma once

#include "../sdl/SoloSDLDevice.h"


namespace solo
{
    class SDLOpenGLDevice final: public SDLDevice
    {
    public:
        explicit SDLOpenGLDevice(const DeviceCreationArgs& args);
        virtual ~SDLOpenGLDevice();

        virtual void saveScreenshot(const std::string& path) override final;

        virtual auto getCanvasSize() const -> Vector2 override final;

    private:
        virtual void endUpdate() override final;

        SDL_GLContext context = nullptr;
    };
}
