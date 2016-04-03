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


auto StubDevice::getWindowTitle() const -> std::string
{
    return windowTitle;
}


void StubDevice::setCursorCaptured(bool captured)
{
}


auto StubDevice::getCanvasSize() const -> Vector2
{
    return Vector2(0, 0);
}


auto StubDevice::getLifetime() const -> float
{
    return 0;
}
