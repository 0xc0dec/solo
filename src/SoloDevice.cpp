#include "SoloDevice.h"
#include "platform/SoloSDLOpenGLDevice.h"

using namespace solo;


Device::Device(const EngineCreationArgs& args):
	close(false), creationArgs(args)
{
}


bool Device::closeRequested() const
{
	return close;
}


ptr<Device> DeviceFactory::create(const EngineCreationArgs& args)
{
	return NEW2(SDLOpenGLDevice, args);
}