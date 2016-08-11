#pragma once

#include "SoloDevice.h"

namespace solo
{
    class StubDevice final: public Device
    {
    public:
        explicit StubDevice(const DeviceSetup& setup): Device(setup) {}

        void setWindowTitle(const std::string& title) override final;
        auto getWindowTitle() const -> std::string override final;
        void saveScreenshot(const std::string& path) override final {}
        void setCursorCaptured(bool captured) override final {}
        auto getCanvasSize() const -> Vector2 override final;
        auto getLifetime() const -> float override final;

    private:
        void beginUpdate() override final {}
        void endUpdate() override final {}

        std::string windowTitle;
    };

    inline void StubDevice::setWindowTitle(const std::string& title)
    {
        windowTitle = title;
    }

    inline auto StubDevice::getWindowTitle() const -> std::string
    {
        return windowTitle;
    }

    inline auto StubDevice::getCanvasSize() const -> Vector2
    {
        return Vector2(0, 0);
    }

    inline auto StubDevice::getLifetime() const -> float 
    {
        return 0;
    }
}