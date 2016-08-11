#pragma once

#include "SoloDevice.h"

namespace solo
{
    class StubDevice final: public Device
    {
    public:
        explicit StubDevice(const DeviceSetup& setup);

        void setWindowTitle(const std::string& title) override final { windowTitle = title; }
        auto getWindowTitle() const -> std::string override final { return windowTitle; }

        void saveScreenshot(const std::string& path) override final {}

        void setCursorCaptured(bool captured) override final;

        auto getCanvasSize() const -> Vector2 override final { return Vector2(0, 0); }

        auto getLifetime() const -> float override final { return 0; }

    private:
        void beginUpdate() override final;
        void endUpdate() override final;

        std::string windowTitle;
    };
}