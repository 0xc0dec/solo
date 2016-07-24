#pragma once

#include "SoloCommon.h"
#include "SoloVector2.h"
#include "SoloDeviceCreationArgs.h"
#include <unordered_map>
#include <unordered_set>


namespace solo
{
    class Scene;
    class AssetLoader;
    class FileSystem;
    class Renderer;
    class Graphics;
    class Physics;
    class Logger;

    enum class KeyCode
    {
        Digit0,
        Digit1,
        Digit2,
        Digit3,
        Digit4,
        Digit5,
        Digit6,
        Digit7,
        Digit8,
        Digit9,
        Q, W, E, R, T, Y, U, I, O, P,
        A, S, D, F, G, H, J, K, L,
        Z, X, C, V, B, N, M,
        Escape,
        LeftArrow,
        RightArrow,
        UpArrow,
        DownArrow,
        Backspace,
        Space,
        Return,
        LeftShift,
        LeftCtrl,
        LeftAlt,
        RightShift,
        RightCtrl,
        RightAlt
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
        OpenGL,
        Vulkan
    };

    class DeviceToken
    {
        friend class Device;
        DeviceToken() {}
    };

    class Device
    {
    public:
        static auto init(const DeviceCreationArgs& args) -> Device*;
        static auto get() -> Device*; // TODO reduce usages of this and prefer DI as much as possible
        static void shutdown();

        virtual ~Device();
        SL_NONCOPYABLE(Device)

        virtual void setWindowTitle(const std::string& title) = 0;
        virtual auto getWindowTitle() const -> std::string = 0;

        virtual void saveScreenshot(const std::string& path) = 0;

        virtual void setCursorCaptured(bool captured) = 0;
        virtual auto getCanvasSize() const -> Vector2 = 0;
        virtual auto getLifetime() const -> float = 0;

        auto getTimeDelta() const -> float;

        bool isKeyPressed(KeyCode code, bool firstTime = false) const;
        bool isKeyReleased(KeyCode code) const;

        auto getMouseMotion() const -> Vector2;
        bool isMouseButtonDown(MouseButton button, bool firstTime = false) const;
        bool isMouseButtonReleased(MouseButton button) const;

        void run();
        void stopRunning();

        auto getMode() const -> DeviceMode;
        auto getScene() const -> Scene*;
        auto getFileSystem() const -> FileSystem*;
        auto getAssetLoader() const -> AssetLoader*;
        auto getRenderer() const -> Renderer*;
        auto getGraphics() const -> Graphics*;
        auto getPhysics() const -> Physics*;
        auto getLogger() const -> Logger*;

    protected:
        explicit Device(const DeviceCreationArgs& args);

        virtual void beginUpdate() = 0;
        virtual void endUpdate() = 0;

        void updateTime();

        DeviceCreationArgs creationArgs;

        sptr<Scene> scene;
        sptr<FileSystem> fs;
        sptr<AssetLoader> assetLoader;
        sptr<Renderer> renderer;
        sptr<Graphics> graphics;
        sptr<Physics> physics;
        sptr<Logger> logger;

        // key code -> was pressed for the first time
        std::unordered_map<KeyCode, bool> pressedKeys;
        std::unordered_set<KeyCode> releasedKeys;

        int32_t mouseDeltaX = 0;
        int32_t mouseDeltaY = 0;
        std::unordered_map<MouseButton, bool> pressedMouseButtons;
        std::unordered_set<MouseButton> releasedMouseButtons;

        bool close = false;
        bool running = false;
        float lastUpdateTime = 0;
        float timeDelta = 0;

    private:
        static uptr<Device> instance;
    };

    inline auto Device::getTimeDelta() const -> float
    {
        return timeDelta;
    }

    inline void Device::stopRunning()
    {
        running = false;
    }

    inline auto Device::getMode() const -> DeviceMode
    {
        return creationArgs.mode;
    }

    inline auto Device::getScene() const -> Scene*
    {
        return scene.get();
    }

    inline auto Device::getFileSystem() const -> FileSystem*
    {
        return fs.get();
    }

    inline auto Device::getRenderer() const -> Renderer*
    {
        return renderer.get();
    }

    inline auto Device::getAssetLoader() const -> AssetLoader*
    {
        return assetLoader.get();
    }

    inline auto Device::getGraphics() const -> Graphics*
    {
        return graphics.get();
    }

    inline auto Device::getPhysics() const -> Physics*
    {
        return physics.get();
    }

    inline auto Device::getLogger() const -> Logger*
    {
        return logger.get();
    }
}
