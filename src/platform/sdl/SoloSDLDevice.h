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
        void setWindowTitle(const std::string& title) override final;

        void setCursorCaptured(bool captured) override final;

        auto getLifetime() const -> float override final;

    protected:
        explicit SDLDevice(const DeviceSetup& setup);

        void beginUpdate() override final;

        SDL_Window* window = nullptr;

        bool hasMouseFocus = false;
        bool hasKeyboardFocus = false;

    private:
        void prepareKeyboardState();
        void prepareMouseState();
        void readWindowState();
        void readEvents();
        void processKeyboardEvent(const SDL_Event& evt);
        void processMouseEvent(const SDL_Event& evt);
        void processWindowEvent(const SDL_Event& evt);
    };
}