#pragma once

#include <string>


namespace solo
{
    enum class DeviceMode;

    class DeviceCreationArgs
    {
    public:
        DeviceMode mode;
        uint32_t canvasWidth = 800;
        uint32_t canvasHeight = 600;
        bool fullScreen = false;
        std::string windowTitle;
        uint32_t redBits = 5;
        uint32_t greenBits = 5;
        uint32_t blueBits = 5;
        uint32_t alphaBits = 0;
        uint32_t depthBits = 24;
        std::string logFilePath;

        auto withMode(DeviceMode mode) -> DeviceCreationArgs&;
        auto withDimensions(uint32_t canvasWidth, uint32_t canvasHeight) -> DeviceCreationArgs&;
        auto withFullScreen(bool fullScreen) -> DeviceCreationArgs&;
        auto withWindowTitle(const std::string& title) -> DeviceCreationArgs&;
        auto withColorBits(uint32_t redBits, uint32_t greenBits, uint32_t blueBits, uint32_t alphaBits) -> DeviceCreationArgs&;
        auto withDepthBits(uint32_t depthBits) -> DeviceCreationArgs&;
        auto withLogPath(const std::string& logPath) -> DeviceCreationArgs&;
    };
}
