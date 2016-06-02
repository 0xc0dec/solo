#pragma once

#include "SoloDevice.h"

namespace solo
{
    class StubDevice: public Device
    {
    public:
        explicit StubDevice(const DeviceCreationArgs& args);

        void setWindowTitle(const std::string& title) override;
        auto getWindowTitle() const -> std::string override;

        void saveScreenshot(const std::string& path) override {}

        void setCursorCaptured(bool captured) override;

        auto getCanvasSize() const -> Vector2 override;

        auto getLifetime() const -> float override;

    private:
        void beginUpdate() override;
        void endUpdate() override;

        std::string windowTitle;
    };
}