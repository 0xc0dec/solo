/*
    Copyright (c) Aleksey Fedotov
    MIT license
*/

#include "SoloFrameBuffer.h"
#include "SoloLuaCommon.h"
#include "SoloLogger.h"
#include "SoloScene.h"
#include "SoloDevice.h"

using namespace solo;

static void registerDevice(CppBindModule<LuaBinding> &module)
{
    auto device = BEGIN_CLASS(module, Device);
    REG_METHOD(device, Device, getWindowTitle);
    REG_METHOD(device, Device, setWindowTitle);
    REG_METHOD(device, Device, getCanvasSize);
    REG_METHOD(device, Device, saveScreenshot);
    REG_METHOD(device, Device, setCursorCaptured);
    REG_METHOD(device, Device, getLifetime);
    REG_METHOD(device, Device, getTimeDelta);
    REG_METHOD(device, Device, isWindowCloseRequested);
    REG_METHOD(device, Device, isQuitRequested);
    REG_METHOD(device, Device, isKeyPressed);
    REG_METHOD(device, Device, isKeyReleased);
    REG_METHOD(device, Device, getMouseMotion);
    REG_METHOD(device, Device, isMouseButtonDown);
    REG_METHOD(device, Device, isMouseButtonReleased);
    REG_METHOD(device, Device, update);
    REG_METHOD(device, Device, getSetup);
    REG_METHOD(device, Device, getFileSystem);
    REG_METHOD(device, Device, getAssetLoader);
    REG_METHOD(device, Device, getRenderer);
    REG_METHOD(device, Device, getPhysics);
    REG_METHOD(device, Device, getLogger);
    device.endClass();
}

static void registerDeviceSetup(CppBindModule<LuaBinding> &module)
{
    auto setup = BEGIN_CLASS(module, DeviceSetup);
    REG_VARIABLE(setup, DeviceSetup, mode);
    REG_VARIABLE(setup, DeviceSetup, canvasWidth);
    REG_VARIABLE(setup, DeviceSetup, canvasHeight);
    REG_VARIABLE(setup, DeviceSetup, fullScreen);
    REG_VARIABLE(setup, DeviceSetup, windowTitle);
    REG_VARIABLE(setup, DeviceSetup, redBits);
    REG_VARIABLE(setup, DeviceSetup, greenBits);
    REG_VARIABLE(setup, DeviceSetup, blueBits);
    REG_VARIABLE(setup, DeviceSetup, depthBits);
    REG_VARIABLE(setup, DeviceSetup, alphaBits);
    REG_VARIABLE(setup, DeviceSetup, vsync);
    REG_VARIABLE(setup, DeviceSetup, logFilePath);
    setup.endClass();
}

void registerDeviceApi(CppBindModule<LuaBinding> &module)
{
    registerDeviceSetup(module);
    registerDevice(module);
}
