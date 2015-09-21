#include "SoloDevice.h"
#include "SoloStubDevice.h"
#include "platform/SoloSDLOpenGLDevice.h"

using namespace solo;


shared<Device> Device::create(const EngineCreationArgs& args)
{
	if (args.mode == EngineMode::OpenGL)
		return NEW2(SDLOpenGLDevice, args);
	return NEW2(StubDevice, args);
}


Device::Device(const EngineCreationArgs& args):
	creationArgs(args)
{
}


bool Device::isKeyPressed(KeyCode code, bool firstTimeOnly) const
{
	auto where = pressedKeys.find(code);
	return where != pressedKeys.end() && (!firstTimeOnly || where->second);
}


bool Device::isKeyReleased(KeyCode code) const
{
	return releasedKeys.find(code) != releasedKeys.end();
}


Vector2 Device::getMouseMotion() const
{
	return Vector2(static_cast<float>(mouseDeltaX), static_cast<float>(mouseDeltaY));
}


bool Device::isMouseButtonDown(MouseButton button, bool firstTimeOnly) const
{
	auto where = pressedMouseButtons.find(button);
	return where != pressedMouseButtons.end() && (!firstTimeOnly || where->second);
}


bool Device::isMouseButtonReleased(MouseButton button) const
{
	return releasedMouseButtons.find(button) != releasedMouseButtons.end();
}


void Device::requestShutdown()
{
	close = true;
}


bool Device::shutdownRequested() const
{
	return close;
}


void Device::updateTime()
{
	auto time = getLifetime();
	timeDelta = time - lastUpdateTime;
	lastUpdateTime = time;
}


float Device::getTimeDelta() const
{
	return timeDelta;
}
