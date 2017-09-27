/*
    Copyright (c) Aleksey Fedotov
    MIT license
*/

#pragma once

#include "SoloCommon.h"
#include "SoloVector2.h"
#include "SoloDeviceSetup.h"
#include <unordered_map>
#include <unordered_set>
#include <functional>

namespace solo
{
    class Scene;
    class FileSystem;
    class Renderer;
    class Physics;
    class ScriptRuntime;
    class Logger;
    class JobPool;

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
        Null,
        OpenGL,
        Vulkan
    };

    class Device
    {
    public:
        SL_DISABLE_COPY_AND_MOVE(Device)

        static auto create(const DeviceSetup &setup) -> uptr<Device>;

        virtual ~Device() {}

        virtual auto getWindowTitle() const -> std::string = 0;
        virtual void setWindowTitle(const std::string &title) = 0;

        virtual auto getCanvasSize() const -> Vector2 = 0;

        virtual void saveScreenshot(const std::string &path) = 0; // TODO return bytes?

        virtual void setCursorCaptured(bool captured) = 0;

        virtual auto getLifetime() const -> float = 0;
        auto getTimeDelta() const -> float { return timeDelta; }

        bool isWindowCloseRequested() const { return windowCloseRequested; }
        bool isQuitRequested() const { return quitRequested; }

        bool isKeyPressed(KeyCode code, bool firstTime = false) const;
        bool isKeyReleased(KeyCode code) const;

        auto getMouseMotion() const -> Vector2;
        bool isMouseButtonDown(MouseButton button, bool firstTime = false) const;
        bool isMouseButtonReleased(MouseButton button) const;

        void update(std::function<void()> update);

        auto getSetup() const -> DeviceSetup { return setup; }
        auto getMode() const -> DeviceMode { return setup.mode; }

        auto getFileSystem() const -> FileSystem* { return fs.get(); }
        auto getRenderer() const -> Renderer* { return renderer.get(); }
        auto getPhysics() const -> Physics* { return physics.get(); }
        auto getScriptRuntime() const -> ScriptRuntime* { return scriptRuntime.get(); }
        auto getLogger() const -> Logger* { return logger.get(); }
        auto getJobPool() const -> JobPool* { return jobPool.get(); }

    protected:
        DeviceSetup setup;

        sptr<Logger> logger;
        sptr<Renderer> renderer;
        sptr<Physics> physics;
        sptr<FileSystem> fs;
        sptr<ScriptRuntime> scriptRuntime;
        sptr<JobPool> jobPool;

        // key code -> was pressed for the first time
        std::unordered_map<KeyCode, bool> pressedKeys;
        std::unordered_set<KeyCode> releasedKeys;

        int32_t mouseDeltaX = 0;
        int32_t mouseDeltaY = 0;
        std::unordered_map<MouseButton, bool> pressedMouseButtons;
        std::unordered_set<MouseButton> releasedMouseButtons;

        float lastUpdateTime = 0;
        float timeDelta = 0;

        bool windowCloseRequested = false;
        bool quitRequested = false;

        explicit Device(const DeviceSetup &setup);
        
        virtual void beginUpdate() = 0;
        virtual void endUpdate() = 0;

        void initSubsystems();
        void cleanupSubsystems();
        void updateTime();
    };
}
