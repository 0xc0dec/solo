/*
    Copyright (c) Aleksey Fedotov
    MIT license
*/

#include "SoloFrameBuffer.h"
#include "SoloLuaCommon.h"
#include "SoloScene.h"
#include "SoloDevice.h"
#include "SoloDeviceSetup.h"

using namespace solo;

static void registerDevice(CppBindModule<LuaBinding> &module)
{
    auto device = BEGIN_CLASS(module, Device);
    REG_METHOD(device, Device, windowTitle);
    REG_METHOD(device, Device, setWindowTitle);
    REG_METHOD(device, Device, canvasSize);
    REG_METHOD(device, Device, dpiIndependentCanvasSize);
    REG_METHOD(device, Device, isVsync);
    REG_METHOD(device, Device, mode);
    REG_METHOD(device, Device, saveScreenshot);
    REG_METHOD(device, Device, setCursorCaptured);
    REG_METHOD(device, Device, lifetime);
    REG_METHOD(device, Device, timeDelta);
    REG_METHOD(device, Device, isWindowCloseRequested);
    REG_METHOD(device, Device, isQuitRequested);
    REG_METHOD(device, Device, isKeyPressed);
    REG_METHOD(device, Device, isKeyReleased);
    REG_METHOD(device, Device, mouseMotion);
    REG_METHOD(device, Device, mousePosition);
    REG_METHOD(device, Device, isMouseButtonDown);
    REG_METHOD(device, Device, isMouseButtonReleased);
    REG_METHOD(device, Device, update);
    REG_METHOD(device, Device, hasActiveBackgroundJobs);
    REG_METHOD(device, Device, fileSystem);
    REG_METHOD(device, Device, physics);
    device.endClass();
}

void registerDeviceSetup(CppBindModule<LuaBinding> &module)
{
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

void registerDeviceApi(CppBindModule<LuaBinding> &module)
{
    registerDeviceSetup(module);
    registerDevice(module);
}
