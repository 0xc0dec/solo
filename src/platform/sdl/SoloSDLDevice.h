#pragma once

#include "SoloDevice.h"
#include <SDL.h>


namespace solo
{
    class SDLDevice: public Device
    {
    public:
        virtual auto getWindowTitle() const->std::string override final;
        virtual void setWindowTitle(const std::string& title) override final;

        virtual void setCursorCaptured(bool captured) override final;

        virtual auto getLifetime() const -> float override final;

    protected:
        explicit SDLDevice(const DeviceCreationArgs& args);

        virtual void beginUpdate() override final;

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