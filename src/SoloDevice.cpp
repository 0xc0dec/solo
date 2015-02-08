#include "SoloDevice.h"
#include "platform/SoloSDLOpenGLDevice.h"

using namespace solo;


Device::Device(const EngineCreationArgs& args):
	close(false), creationArgs(args)
{
}


ptr<Device> Device::create(const EngineCreationArgs& args)
{
	return SDLOpenGLDevice::create(args);
}


bool Device::closeRequested() const
{
	return close;
}