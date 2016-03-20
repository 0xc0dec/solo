#pragma once

#include "SoloDevice.h"

namespace solo
{
    class StubDevice: public Device
    {
    public:
        virtual void setWindowTitle(const std::string& title) override;
        virtual std::string getWindowTitle() const override;

        virtual void setCursorCaptured(bool captured) override;

        virtual Vector2 getCanvasSize() const override;

        virtual float getLifetime() const override;

    private:
        friend class Device;

        virtual void beginUpdate() override;
        virtual void endUpdate() override;

        explicit StubDevice(const DeviceCreationArgs& args);

        std::string windowTitle;
    };
}