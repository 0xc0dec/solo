/*
    Copyright (c) Aleksey Fedotov
    MIT license
*/

#pragma once

#include <string>


namespace solo
{
    enum class DeviceMode;

    // class because lua binding doesn't like structs :(
    class DeviceSetup
    {
    public:
        DeviceMode mode;
        
        uint32_t canvasWidth = 800;
        uint32_t canvasHeight = 600;
        uint32_t redBits = 5;
        uint32_t greenBits = 5;
        uint32_t blueBits = 5;
        uint32_t depthBits = 24;
        uint32_t alphaBits = 0;
        
        bool fullScreen = false;
        bool vsync = false;
        
        std::string windowTitle;
        std::string logFilePath = "";
    };
}
