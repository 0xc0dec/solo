#pragma once

#include "SoloDevice.h"
#include <SDL.h>


namespace solo
{
    class SDLOpenGLDevice final: public Device
    {
    public:
        virtual ~SDLOpenGLDevice();

        virtual void setWindowTitle(const std::string& title) override final;
        virtual std::string getWindowTitle() const override final;

        virtual void setCursorCaptured(bool captured) override final;

        virtual float getLifetime() const override final;

        virtual Vector2 getCanvasSize() const override final;

    private:
        friend class Device;

        explicit SDLOpenGLDevice(const DeviceCreationArgs& args);

        virtual void beginUpdate() override final;
        virtual void endUpdate() override final;

        void prepareKeyboardState();
        void prepareMouseState();

        void readWindowState();
        void readEvents();
        void processKeyboardEvent(const SDL_Event& evt);
        void processMouseEvent(const SDL_Event& evt);
        void processWindowEvent(const SDL_Event& evt);

        bool hasMouseFocus = false;
        bool hasKeyboardFocus = false;

        SDL_Window* window = nullptr;
        SDL_GLContext context = nullptr;

        std::tuple<uint32_t, uint32_t> selectContextVersion();
    };
}
