#include "SoloDevice.h"
#include "platform/SoloSDLOpenGLDevice.h"

using namespace solo;


Device::Device(const EngineCreationArgs& args):
	close(false), creationArgs(args)
{
}


bool Device::isKeyPressed(KeyCode code, bool repeat) const
{
	auto where = pressedKeys.find(code);
	return where != pressedKeys.end() && where->second == repeat;
}


bool Device::isKeyReleased(KeyCode code) const
{
	return releasedKeys.find(code) != releasedKeys.end();
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