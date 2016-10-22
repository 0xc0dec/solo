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

#include "SoloCommon.h"
#include <string>


namespace solo
{
    enum class DeviceMode;

    SL_FLUENT_DTO(DeviceSetup,
        SL_FLUENT_DTO_FIELD(DeviceMode, DeviceMode, mode, withMode, )
        SL_FLUENT_DTO_FIELD(uint32_t, uint32_t, canvasWidth, withCanvasWidth, = 800)
        SL_FLUENT_DTO_FIELD(uint32_t, uint32_t, canvasHeight, withCanvasHeight, = 600)
        SL_FLUENT_DTO_FIELD(uint32_t, uint32_t, fullScreen, withFullScreen, = false)
        SL_FLUENT_DTO_FIELD(std::string, const std::string&, windowTitle, withWindowTitle, )
        SL_FLUENT_DTO_FIELD(uint32_t, uint32_t, redBits, withRedBits, = 5)
        SL_FLUENT_DTO_FIELD(uint32_t, uint32_t, greenBits, withGreenBits, = 5)
        SL_FLUENT_DTO_FIELD(uint32_t, uint32_t, blueBits, withBlueBits, = 5)
        SL_FLUENT_DTO_FIELD(uint32_t, uint32_t, alphaBits, withAlphaBits, = 0)
        SL_FLUENT_DTO_FIELD(uint32_t, uint32_t, depthBits, withDepthBits, = 24)
        SL_FLUENT_DTO_FIELD(bool, bool, vsync, withVsync, = false)
        SL_FLUENT_DTO_FIELD(std::string, const std::string&, logFilePath, withLogFilePath, )

        auto withDimensions(uint32_t canvasWidth, uint32_t canvasHeight) -> DeviceSetup&
        {
            this->canvasWidth = canvasWidth;
            this->canvasHeight = canvasHeight;
            return *this;
        }
    )
}
