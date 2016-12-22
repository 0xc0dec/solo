/*
    Copyright (c) Aleksey Fedotov

    This software is provided 'as-is', without any express or implied
    warranty. In no event will the authors be held liable for any damages
    arising from the use of this software.

    Permission is granted to anyone to use this software for any purpose,
    including commercial applications, and to alter it and redistribute it
    freely, subject to the following restrictions:

    1. The origin of this software must not be misrepresented; you must not
        claim that you wrote the original software. If you use this software
        in a product, an acknowledgment in the product documentation would be
        appreciated but is not required.
    2. Altered source versions must be plainly marked as such, and must not be
        misrepresented as being the original software.
    3. This notice may not be removed or altered from any source distribution.
*/

#pragma once

#include "SoloDevice.h"

namespace solo
{
    class NullDevice final: public Device
    {
    public:
        explicit NullDevice(const DeviceSetup &setup): Device(setup) {}

        void setWindowTitle(const std::string &title) override final;
        auto getWindowTitle() const -> std::string override final;
        void saveScreenshot(const std::string &path) override final {}
        void setCursorCaptured(bool captured) override final {}
        auto getCanvasSize() const -> Vector2 override final;
        auto getLifetime() const -> float override final;

    private:
        void beginUpdate() override final {}
        void endUpdate() override final {}

        std::string windowTitle;
    };

    inline void NullDevice::setWindowTitle(const std::string &title)
    {
        windowTitle = title;
    }

    inline auto NullDevice::getWindowTitle() const -> std::string
    {
        return windowTitle;
    }

    inline auto NullDevice::getCanvasSize() const -> Vector2
    {
        return Vector2(0, 0);
    }

    inline auto NullDevice::getLifetime() const -> float
    {
        return 0;
    }
}