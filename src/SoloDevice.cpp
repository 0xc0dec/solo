#include "SoloDevice.h"

using namespace solo;


Device::Device(EngineCreationArgs args)
	: _closeRequested(false),
	_creationArgs(args)
{
}


Device::~Device()
{
}