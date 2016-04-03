#pragma once

#include "SoloDevice.h"

namespace solo
{
    class StubDevice: public Device
    {
    public:
        virtual void setWindowTitle(const std::string& title) override;
        virtual auto getWindowTitle() const -> std::string override;

        virtual void saveScreenshot(const std::string& path) override {}

        virtual void setCursorCaptured(bool captured) override;

        virtual auto getCanvasSize() const -> Vector2 override;

        virtual auto getLifetime() const -> float override;

    private:
        friend class Device;

        explicit StubDevice(const DeviceCreationArgs& args);

        virtual void beginUpdate() override;
        virtual void endUpdate() override;

        std::string windowTitle;
    };
}