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
	REG_METHOD(device, Device, getWindowTitle);
	REG_METHOD(device, Device, setWindowTitle);
	REG_METHOD(device, Device, getCanvasSize);
	REG_METHOD(device, Device, getDpiIndependentCanvasSize);
	REG_METHOD(device, Device, isVsync);
	REG_METHOD(device, Device, getMode);
	REG_METHOD(device, Device, saveScreenshot);
	REG_METHOD(device, Device, setCursorCaptured);
	REG_METHOD(device, Device, getLifetime);
	REG_METHOD(device, Device, getTimeDelta);
	REG_METHOD(device, Device, isWindowCloseRequested);
	REG_METHOD(device, Device, isQuitRequested);
	REG_METHOD(device, Device, isKeyPressed);
	REG_METHOD(device, Device, isKeyReleased);
	REG_METHOD(device, Device, getMouseMotion);
	REG_METHOD(device, Device, getMousePosition);
	REG_METHOD(device, Device, isMouseButtonDown);
	REG_METHOD(device, Device, isMouseButtonReleased);
	REG_METHOD(device, Device, update);
	REG_METHOD(device, Device, hasActiveBackgroundJobs);
	REG_METHOD(device, Device, getFileSystem);
	REG_METHOD(device, Device, getPhysics);
	REG_METHOD(device, Device, getLogger);
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

void registerDeviceSetup2(sol::table &module)
{
	module.new_usertype<DeviceSetup>(
		"DeviceSetup",
		"mode", &DeviceSetup::mode,
		"canvasWidth", &DeviceSetup::canvasWidth,
		"canvasHeight", &DeviceSetup::canvasHeight,
		"fullScreen", &DeviceSetup::fullScreen,
		"windowTitle", &DeviceSetup::windowTitle,
		"vsync", &DeviceSetup::vsync,
		"logFilePath", &DeviceSetup::logFilePath
	);
}

void registerDevice2(sol::table &module)
{
	module.new_usertype<Device>(
		"Device",
		"getWindowTitle", &Device::getWindowTitle,
		"setWindowTitle", &Device::setWindowTitle,
		"getCanvasSize", &Device::getCanvasSize,
		"getDpiIndependentCanvasSize", &Device::getDpiIndependentCanvasSize,
		"isVsync", &Device::isVsync,
		"getMode", &Device::getMode,
		"saveScreenshot", &Device::saveScreenshot,
		"setCursorCaptured", &Device::setCursorCaptured,
		"getLifetime", &Device::getLifetime,
		"getTimeDelta", &Device::getTimeDelta,
		"isWindowCloseRequested", &Device::isWindowCloseRequested,
		"isQuitRequested", &Device::isQuitRequested,
		"isKeyPressed", &Device::isKeyPressed,
		"isKeyReleased", &Device::isKeyReleased,
		"getMouseMotion", &Device::getMouseMotion,
		"getMousePosition", &Device::getMousePosition,
		"isMouseButtonDown", &Device::isMouseButtonDown,
		"isMouseButtonReleased", &Device::isMouseButtonReleased,
		"update", &Device::update,
		"hasActiveBackgroundJobs", &Device::hasActiveBackgroundJobs,
		"getFileSystem", &Device::getFileSystem,
		"getPhysics", &Device::getPhysics,
		"getLogger", &Device::getLogger
	);
}

void registerDeviceApi2(sol::table &module)
{
	registerDeviceSetup2(module);
	registerDevice2(module);
}