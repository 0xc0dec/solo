/* 
 * Copyright (c) Aleksey Fedotov 
 * MIT license 
 */

#pragma once

#include "SoloCommon.h"

namespace solo
{
    enum class DeviceMode;

    // "class" because binding to lua seems to not support structs
    class DeviceSetup
    {
    public:
        DeviceMode mode;
        
        u32 canvasWidth = 800;
        u32 canvasHeight = 600;
        
        bool fullScreen = false;
        bool vsync = false;
        
        str windowTitle;
        str logFilePath = "";
    };
}
