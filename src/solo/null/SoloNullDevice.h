/*
    Copyright (c) Aleksey Fedotov
    MIT license
*/

#pragma once

#include "SoloDevice.h"

namespace solo
{
    namespace null
    {
        class NullDevice final : public Device
        {
        public:
            explicit NullDevice(const DeviceSetup &setup) : Device(setup) {}

            void setWindowTitle(const str &title) override final { windowTitle = title; }
            auto getWindowTitle() const -> str override final { return windowTitle; }
            void saveScreenshot(const str &path) override final {}
            void setCursorCaptured(bool captured) override final {}
            auto getCanvasSize() const -> Vector2 override final { return{0, 0}; }
            auto getLifetime() const -> float override final { return 0; }

        private:
            str windowTitle;

            void beginUpdate() override final {}
            void endUpdate() override final {}
        };
    }
}