#pragma once

#include "SoloBase.h"
#include "SoloVector2.h"
#include <functional>
#include <unordered_map>
#include <unordered_set>


namespace solo
{
    class Scene;
    class ResourceManager;
    class FileSystem;
    class Renderer;
    class Graphics;
    class Logger;

    enum class KeyCode
    {
        Q, W, E, R, T, Y, U, I, O, P,
        A, S, D, F, G, H, J, K, L,
        Z, X, C, V, B, N, M,
        Escape,
        LeftArrow,
        RightArrow,
        UpArrow,
        DownArrow,
        Backspace,
        Space
        // ...
    };

    enum class MouseButton
    {
        Left,
        Middle,
        Right
    };

    enum class DeviceMode
    {
        Stub,
        OpenGL
    };

    struct DeviceCreationArgs
    {
        DeviceMode mode;
        uint32_t canvasWidth;
        uint32_t canvasHeight;
        bool fullScreen;
        std::string windowTitle;
        uint32_t bits;
        uint32_t depth;
        std::string logFilePath;

        DeviceCreationArgs(
            DeviceMode mode = DeviceMode::OpenGL,
            uint32_t canvasWidth = 800,
            uint32_t canvasHeight = 600,
            bool fullScreen = false,
            const std::string& windowTitle = "",
            uint32_t bits = 32,
            uint32_t depth = 16,
            const std::string& logFilePath = "") :
            mode(mode),
            canvasWidth(canvasWidth > 0 ? canvasWidth : 1),
            canvasHeight(canvasHeight > 0 ? canvasHeight : 1),
            fullScreen(fullScreen),
            windowTitle(windowTitle),
            bits(bits > 0 ? bits : 32),
            depth(depth > 0 ? depth : 16),
            logFilePath(logFilePath)
        {
        }
    };

    class Device
    {
    public:
        static shared<Device> create(const DeviceCreationArgs& args);

        SL_NONCOPYABLE(Device)
        virtual ~Device();

        virtual void beginUpdate() = 0;
        virtual void endUpdate() = 0;

        virtual void setWindowTitle(const std::string& title) = 0;
        virtual std::string getWindowTitle() const = 0;

        virtual void setCursorCaptured(bool captured) = 0;

        virtual Vector2 getCanvasSize() const = 0;

        virtual float getLifetime() const = 0;

        float getTimeDelta() const;

        bool isKeyPressed(KeyCode code, bool firstTimeOnly = false) const;
        bool isKeyReleased(KeyCode code) const;

        Vector2 getMouseMotion() const;
        bool isMouseButtonDown(MouseButton button, bool firstTimeOnly = false) const;
        bool isMouseButtonReleased(MouseButton button) const;

        void run();
        void reset();

        void requestShutdown();
        bool shutdownRequested() const;

        DeviceMode getMode() const;

        Scene* getScene() const;
        FileSystem* getFileSystem() const;
        ResourceManager* getResourceManager() const;
        Renderer* getRenderer() const;
        Graphics* getGraphics() const;
        Logger* getLogger() const;

    protected:
        explicit Device(const DeviceCreationArgs& args);

        void updateTime();
        void startSystems();
        void stopSystems();

        DeviceCreationArgs creationArgs;

        shared<Scene> scene;
        shared<FileSystem> fs;
        shared<ResourceManager> resourceManager;
        shared<Renderer> renderer;
        shared<Graphics> graphics;
        shared<Logger> logger;

        // stores what keys were pressed and if it was a repeat
        std::unordered_map<KeyCode, bool> pressedKeys;
        std::unordered_set<KeyCode> releasedKeys;

        int32_t mouseDeltaX = 0;
        int32_t mouseDeltaY = 0;
        std::unordered_map<MouseButton, bool> pressedMouseButtons;
        std::unordered_set<MouseButton> releasedMouseButtons;

        bool close = false;
        bool shutdown = false;
        float lastUpdateTime = 0;
        float timeDelta = 0;
    };

    inline float Device::getTimeDelta() const
    {
        return timeDelta;
    }

    inline void Device::requestShutdown()
    {
        shutdown = true;
    }

    inline bool Device::shutdownRequested() const
    {
        return shutdown;
    }

    inline DeviceMode Device::getMode() const
    {
        return creationArgs.mode;
    }

    inline Scene* Device::getScene() const
    {
        return scene.get();
    }

    inline FileSystem* Device::getFileSystem() const
    {
        return fs.get();
    }

    inline Renderer* Device::getRenderer() const
    {
        return renderer.get();
    }

    inline ResourceManager* Device::getResourceManager() const
    {
        return resourceManager.get();
    }

    inline Graphics* Device::getGraphics() const
    {
        return graphics.get();
    }

    inline Logger* Device::getLogger() const
    {
        return logger.get();
    }
}
