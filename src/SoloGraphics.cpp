#include "SoloGraphics.h"
#include "SoloDevice.h"
#include "platform/opengl/SoloOpenGLGraphics.h"
#include "platform/stub/SoloStubGraphics.h"

using namespace solo;


shared<Graphics> GraphicsFactory::create(Device* device)
{
	if (device->getMode() == DeviceMode::OpenGL)
		return SL_NEW2(OpenGLGraphics, device);
	return SL_NEW2(StubGraphics, device);
}


Graphics::Graphics(Device *device):
	device(device)
{
}
