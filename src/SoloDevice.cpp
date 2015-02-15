#include "SoloDevice.h"
#include "platform/SoloSDLOpenGLDevice.h"

using namespace solo;


Device::Device(const EngineCreationArgs& args):
	mouseDeltaX(0),
	mouseDeltaY(0),
	close(false), creationArgs(args)
{
}


bool Device::isKeyPressed(KeyCode code, bool firstTime) const
{
	auto where = pressedKeys.find(code);
	return where != pressedKeys.end() && where->second == firstTime;
}


bool Device::isKeyReleased(KeyCode code) const
{
	return releasedKeys.find(code) != releasedKeys.end();
}


Vector2 Device::getMouseMotion() const
{
	return Vector2(static_cast<float>(mouseDeltaX), static_cast<float>(mouseDeltaY));
}


bool Device::isMouseButtonPressed(MouseButton button, bool firstTime) const
{
	auto where = pressedMouseButtons.find(button);
	return where != pressedMouseButtons.end() && where->second == firstTime;
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


shared<Device> DeviceFactory::create(const EngineCreationArgs& args)
{
	return NEW2(SDLOpenGLDevice, args);
}