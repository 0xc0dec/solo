#include "SoloStubDevice.h"

using namespace solo;


StubDevice::StubDevice(const DeviceCreationArgs& args):
	Device(args)
{
}


void StubDevice::beginUpdate()
{
}


void StubDevice::endUpdate()
{
}


void StubDevice::setWindowTitle(const std::string& title)
{
	windowTitle = title;
}


std::string StubDevice::getWindowTitle() const
{
	return windowTitle;
}


void StubDevice::setCursorCaptured(bool captured)
{
}


Vector2 StubDevice::getCanvasSize() const
{
	return Vector2(0, 0);
}


float StubDevice::getLifetime() const
{
	return 0;
}
