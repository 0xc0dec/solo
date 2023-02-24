/*
 * Copyright (c) Aleksey Fedotov
 * MIT license
 */

#include "SoloFrameBuffer.h"
#include "SoloLuaCommon.h"
#include "SoloScene.h"
#include "SoloDevice.h"
#include "SoloDeviceSetup.h"

using namespace solo;

static void registerDevice(CppBindModule<LuaBinding> &module) {
    auto binding = BEGIN_CLASS(module, Device);
    REG_METHOD(binding, Device, windowTitle);
    REG_METHOD(binding, Device, setWindowTitle);
    REG_METHOD(binding, Device, canvasSize);
    REG_METHOD(binding, Device, dpiIndependentCanvasSize);
    REG_METHOD(binding, Device, isVsync);
    REG_METHOD(binding, Device, mode);
    REG_METHOD(binding, Device, setCursorCaptured);
    REG_METHOD(binding, Device, lifetime);
    REG_METHOD(binding, Device, timeDelta);
    REG_METHOD(binding, Device, isWindowCloseRequested);
    REG_METHOD(binding, Device, isQuitRequested);
    REG_METHOD(binding, Device, isKeyPressed);
    REG_METHOD(binding, Device, isKeyReleased);
    REG_METHOD(binding, Device, mouseMotion);
    REG_METHOD(binding, Device, mousePosition);
    REG_METHOD(binding, Device, isMouseButtonDown);
    REG_METHOD(binding, Device, isMouseButtonReleased);
    REG_METHOD(binding, Device, update);
    REG_METHOD(binding, Device, hasActiveBackgroundJobs);
    REG_METHOD(binding, Device, fileSystem);
    REG_METHOD(binding, Device, physics);
    REG_METHOD(binding, Device, renderer);
    REG_METHOD(binding, Device, debugInterface);
    REG_PTR_EQUALITY(binding, Device);
    binding.endClass();
}

void registerDeviceSetup(CppBindModule<LuaBinding> &module) {
    auto setup = BEGIN_CLASS(module, DeviceSetup);
    REG_CTOR(setup);
    REG_FIELD(setup, DeviceSetup, mode);
    REG_FIELD(setup, DeviceSetup, canvasWidth);
    REG_FIELD(setup, DeviceSetup, canvasHeight);
    REG_FIELD(setup, DeviceSetup, fullScreen);
    REG_FIELD(setup, DeviceSetup, windowTitle);
    REG_FIELD(setup, DeviceSetup, vsync);
    REG_FIELD(setup, DeviceSetup, logFilePath);
    setup.endClass();
}

void registerDeviceApi(CppBindModule<LuaBinding> &module) {
    registerDeviceSetup(module);
    registerDevice(module);
}
