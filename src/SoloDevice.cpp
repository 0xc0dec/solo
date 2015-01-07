#include "SoloDevice.h"
#include "platform/SoloSDLOpenGLDevice.h"

using namespace solo;


Device::Device(const EngineCreationArgs& args)
	: _closeRequested(false),
	_creationArgs(args)
{
}


ptr<Device> Device::create(const EngineCreationArgs& args)
{
	return SDLOpenGLDevice::create(args);
}


bool Device::closeRequested() const
{
	return _closeRequested;
}