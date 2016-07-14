#pragma once

#include "SoloDevice.h"

namespace solo
{
    class StubDevice final: public Device
    {
    public:
        explicit StubDevice(const DeviceCreationArgs& args);

        void setWindowTitle(const std::string& title) override final;
        auto getWindowTitle() const -> std::string override final;

        void saveScreenshot(const std::string& path) override final {}

        void setCursorCaptured(bool captured) override final;

        auto getCanvasSize() const -> Vector2 override final;

        auto getLifetime() const -> float override final;

    private:
        void beginUpdate() override final;
        void endUpdate() override final;

        std::string windowTitle;
    };
}