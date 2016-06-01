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
        uint32_t pixelBits = 32; // TODO fix: this is not used
        uint32_t depthBits = 24;
        std::string logFilePath;

        auto withMode(DeviceMode mode) -> DeviceCreationArgs&;
        auto withDimensions(uint32_t canvasWidth, uint32_t canvasHeight) -> DeviceCreationArgs&;
        auto withFullScreen(bool fullScreen) -> DeviceCreationArgs&;
        auto withWindowTitle(const std::string& title) -> DeviceCreationArgs&;
        auto withBitsPerPixel(uint32_t bits) -> DeviceCreationArgs&;
        auto withDepthBits(uint32_t depthBits) -> DeviceCreationArgs&;
        auto withLogPath(const std::string& logPath) -> DeviceCreationArgs&;
    };
}
