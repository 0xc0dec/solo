#pragma once

#include "SoloDevice.h"
#include <SDL.h>


namespace solo
{
    class SDLDevice final: public Device
    {
    public:
        virtual auto getWindowTitle() const->std::string override final;
        virtual void setWindowTitle(const std::string& title) override final;

        virtual void setCursorCaptured(bool captured) override final;

    private:
        friend class Device;

        explicit SDLDevice(const DeviceCreationArgs& args);

        SDL_Window* window = nullptr;
    };
}