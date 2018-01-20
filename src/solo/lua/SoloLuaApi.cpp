/*
	Copyright (c) Aleksey Fedotov
	MIT license
*/

#include "SoloLuaCommon.h"
#include "SoloDevice.h"
#include "SoloFileSystem.h"
#include "SoloPhysics.h"
#include "SoloLogger.h"

using namespace solo;

#define FIELD(klass, name) #name, &klass::name
#define ENUM_FIELD(e, name) #name, e::name

static void registerEnums(sol::table &module)
{
	module.new_enum(
		"DeviceMode",
		ENUM_FIELD(DeviceMode, Null),
		ENUM_FIELD(DeviceMode, OpenGL),
		ENUM_FIELD(DeviceMode, Vulkan)
	);
}

static void registerLogger(sol::table &module)
{
	auto logger = module.new_usertype<Logger>(
		"Logger",
		FIELD(Logger, setTargetFile),
		FIELD(Logger, logDebug),
		FIELD(Logger, logInfo),
		FIELD(Logger, logWarning),
		FIELD(Logger, logError),
		FIELD(Logger, logCritical)
	);
}

static void registerDeviceSetup(sol::table &module)
{
	auto deviceSetup = module.new_usertype<DeviceSetup>(
		"DeviceSetup",
		FIELD(DeviceSetup, mode),
		FIELD(DeviceSetup, canvasWidth),
		FIELD(DeviceSetup, canvasHeight),
		FIELD(DeviceSetup, fullScreen),
		FIELD(DeviceSetup, windowTitle),
		FIELD(DeviceSetup, vsync),
		FIELD(DeviceSetup, logFilePath)
	);
}

static void registerDevice(sol::table &module)
{
	module.new_usertype<Device>(
		"Device",
		FIELD(Device, getWindowTitle),
		FIELD(Device, setWindowTitle),
		FIELD(Device, getCanvasSize),
		FIELD(Device, getDpiIndependentCanvasSize),
		FIELD(Device, isVsync),
		FIELD(Device, getMode),
		FIELD(Device, saveScreenshot),
		FIELD(Device, setCursorCaptured),
		FIELD(Device, getLifetime),
		FIELD(Device, getTimeDelta),
		FIELD(Device, isWindowCloseRequested),
		FIELD(Device, isQuitRequested),
		FIELD(Device, isKeyPressed),
		FIELD(Device, isKeyReleased),
		FIELD(Device, getMouseMotion),
		FIELD(Device, getMousePosition),
		FIELD(Device, isMouseButtonDown),
		FIELD(Device, isMouseButtonReleased),
		FIELD(Device, update),
		FIELD(Device, hasActiveBackgroundJobs),
		FIELD(Device, getFileSystem),
		FIELD(Device, getPhysics),
		FIELD(Device, getLogger)
	);
}

static void registerFileSystem(sol::table &module)
{
	module.new_usertype<FileSystem>(
		"FileSystem",
		FIELD(FileSystem, readBytes),
		FIELD(FileSystem, writeBytes),
		FIELD(FileSystem, readText),
		FIELD(FileSystem, readLines),
		FIELD(FileSystem, writeLines),
		FIELD(FileSystem, iterateLines)
	);
}

void registerApi(sol::table &module)
{
	registerEnums(module);
	registerLogger(module);
	registerFileSystem(module);
	registerDeviceSetup(module);
	registerDevice(module);
}
