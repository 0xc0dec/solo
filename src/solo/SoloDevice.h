/*
    Copyright (c) Aleksey Fedotov
    MIT license
*/

#pragma once

#include "SoloCommon.h"
#include "SoloVector2.h"
#include "SoloDeviceSetup.h"
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

        virtual auto getWindowTitle() const -> str = 0;
        virtual void setWindowTitle(const str &title) = 0;

        virtual auto getCanvasSize() const -> Vector2 = 0;

        virtual void saveScreenshot(const str &path) = 0; // TODO return bytes?

        virtual void setCursorCaptured(bool captured) = 0;

        virtual auto getLifetime() const -> float = 0;
        auto getTimeDelta() const -> float { return timeDelta; }

        bool isWindowCloseRequested() const { return windowCloseRequested; }
        bool isQuitRequested() const { return quitRequested; }
        bool hasActiveBackgroundJobs() const;

        bool isKeyPressed(KeyCode code, bool firstTime = false) const;
        bool isKeyReleased(KeyCode code) const;

        auto getMouseMotion() const -> Vector2;
        bool isMouseButtonDown(MouseButton button, bool firstTime = false) const;
        bool isMouseButtonReleased(MouseButton button) const;

        void update(std::function<void()> update);

        auto getMode() const -> DeviceMode { return mode; }
        bool isVsync() const { return vsync; }

        auto getFileSystem() const -> FileSystem* { return fs.get(); }
        auto getRenderer() const -> Renderer* { return renderer.get(); }
        auto getPhysics() const -> Physics* { return physics.get(); }
        auto getScriptRuntime() const -> ScriptRuntime* { return scriptRuntime.get(); }
        auto getLogger() const -> Logger* { return logger.get(); }
        auto getJobPool() const -> JobPool* { return jobPool.get(); }

    protected:
        sptr<Logger> logger;
        sptr<Renderer> renderer;
        sptr<Physics> physics;
        sptr<FileSystem> fs;
        sptr<ScriptRuntime> scriptRuntime;
        sptr<JobPool> jobPool;

        DeviceMode mode;
        bool vsync;

        // key code -> was pressed for the first time
        umap<KeyCode, bool> pressedKeys;
        uset<KeyCode> releasedKeys;

        s32 mouseDeltaX = 0;
        s32 mouseDeltaY = 0;
        umap<MouseButton, bool> pressedMouseButtons;
        uset<MouseButton> releasedMouseButtons;

        float lastUpdateTime = 0;
        float timeDelta = 0;

        bool windowCloseRequested = false;
        bool quitRequested = false;

        explicit Device(const DeviceSetup &setup);
        
        virtual void beginUpdate() = 0;
        virtual void endUpdate() = 0;

        void initSubsystems(const DeviceSetup &setup);
        void cleanupSubsystems();
        void updateTime();
    };
}
