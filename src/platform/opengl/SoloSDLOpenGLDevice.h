#pragma once

#include "../sdl/SoloSDLDevice.h"


namespace solo
{
    class SDLOpenGLDevice final: public SDLDevice
    {
    public:
        virtual ~SDLOpenGLDevice();

        virtual void saveScreenshot(const std::string& path) override final;

        virtual auto getCanvasSize() const -> Vector2 override final;

    private:
        friend class Device;

        explicit SDLOpenGLDevice(const DeviceCreationArgs& args);

        virtual void endUpdate() override final;

        SDL_GLContext context = nullptr;

        auto selectContextVersion() -> std::tuple<uint32_t, uint32_t>;
    };
}
