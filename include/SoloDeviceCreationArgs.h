#pragma once

#include "SoloBase.h"
#include <string>


namespace solo
{
    enum class DeviceMode;

    SL_FLUENT_DTO(DeviceCreationArgs,
        SL_FLUENT_DTO_FIELD(DeviceMode, mode, withMode, )
        SL_FLUENT_DTO_FIELD(uint32_t, canvasWidth, withCanvasWidth, = 800)
        SL_FLUENT_DTO_FIELD(uint32_t, canvasHeight, withCanvasHeight, = 600)
        SL_FLUENT_DTO_FIELD(uint32_t, fullScreen, withFullScreen, = false)
        SL_FLUENT_DTO_FIELD(std::string, windowTitle, withWindowTitle, )
        SL_FLUENT_DTO_FIELD(uint32_t, redBits, withRedBits, = 5)
        SL_FLUENT_DTO_FIELD(uint32_t, greenBits, withGreenBits, = 5)
        SL_FLUENT_DTO_FIELD(uint32_t, blueBits, withBlueBits, = 5)
        SL_FLUENT_DTO_FIELD(uint32_t, alphaBits, withAlphaBits, = 0)
        SL_FLUENT_DTO_FIELD(uint32_t, depthBits, withDepthBits, = 24)
        SL_FLUENT_DTO_FIELD(std::string, logFilePath, withLogFilePath, )

        auto withDimensions(uint32_t canvasWidth, uint32_t canvasHeight) -> DeviceCreationArgs&
        {
            this->canvasWidth = canvasWidth;
            this->canvasHeight = canvasHeight;
            return *this;
        }
    )
}
