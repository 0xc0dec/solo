/* 
 * Copyright (c) Aleksey Fedotov 
 * MIT license 
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
        OpenGL,
        Vulkan
    };

    class Device: public NoCopyAndMove
    {
    public:
        static auto create(const DeviceSetup &setup) -> uptr<Device>;

        virtual ~Device() = default;

        virtual auto windowTitle() const -> str = 0;
        virtual void setWindowTitle(const str &title) = 0;

        virtual auto canvasSize() const -> Vector2 = 0;
        virtual auto dpiIndependentCanvasSize() const -> Vector2 = 0;

        virtual void saveScreenshot(const str &path) = 0;

        virtual void setCursorCaptured(bool captured) = 0;

        virtual auto lifetime() const -> float = 0;
        auto timeDelta() const -> float { return timeDelta_; }

        bool isWindowCloseRequested() const { return windowCloseRequested_; }
        bool isQuitRequested() const { return quitRequested_; }
        bool hasActiveBackgroundJobs() const;

        bool isKeyPressed(KeyCode code, bool firstTime = false) const;
        bool isKeyReleased(KeyCode code) const;

        auto mouseMotion() const -> Vector2 { return mouseDelta_; }
        auto mousePosition() const -> Vector2 { return mousePos_; }
        bool isMouseButtonDown(MouseButton button, bool firstTime = false) const;
        bool isMouseButtonReleased(MouseButton button) const;

        void update(const std::function<void()> &update);

        auto mode() const -> DeviceMode { return mode_; }
        bool isVsync() const { return vsync_; }

        auto fileSystem() const -> FileSystem* { return fs_.get(); }
        auto renderer() const -> Renderer* { return renderer_.get(); }
        auto physics() const -> Physics* { return physics_.get(); }
        auto scriptRuntime() const -> ScriptRuntime* { return scriptRuntime_.get(); }
        auto jobPool() const -> JobPool* { return jobPool_.get(); }

    protected:
        sptr<Renderer> renderer_;
        sptr<Physics> physics_;
        sptr<FileSystem> fs_;
        sptr<ScriptRuntime> scriptRuntime_;
        sptr<JobPool> jobPool_;

        DeviceMode mode_;
        bool vsync_;

        // key code -> was pressed for the first time
        umap<KeyCode, bool> pressedKeys_;
        uset<KeyCode> releasedKeys_;

        Vector2 mouseDelta_;
        Vector2 mousePos_;
        umap<MouseButton, bool> pressedMouseButtons_;
        uset<MouseButton> releasedMouseButtons_;

        float lastUpdateTime_ = 0;
        float timeDelta_ = 0;

        bool windowCloseRequested_ = false;
        bool quitRequested_ = false;

        explicit Device(const DeviceSetup &setup);
        
        virtual void beginUpdate() = 0;
        virtual void endUpdate() = 0;

        void initSubsystems(const DeviceSetup &setup);
        void cleanupSubsystems();
        void updateTime();
    };
}
