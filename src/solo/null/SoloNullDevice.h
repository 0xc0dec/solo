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
        class Device final : public solo::Device
        {
        public:
            explicit Device(const DeviceSetup &setup) : solo::Device(setup) {}

            void setWindowTitle(const std::string &title) override final;
            auto getWindowTitle() const -> std::string override final;
            void saveScreenshot(const std::string &path) override final {}
            void setCursorCaptured(bool captured) override final {}
            auto getCanvasSize() const -> Vector2 override final;
            auto getLifetime() const -> float override final;

        private:
            std::string windowTitle;

            void beginUpdate() override final {}
            void endUpdate() override final {}
        };

        inline void Device::setWindowTitle(const std::string &title)
        {
            windowTitle = title;
        }

        inline auto Device::getWindowTitle() const -> std::string
        {
            return windowTitle;
        }

        inline auto Device::getCanvasSize() const -> Vector2
        {
            return{0, 0};
        }

        inline auto Device::getLifetime() const -> float
        {
            return 0;
        }
    }
}