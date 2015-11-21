#include "SoloGraphics.h"
#include "SoloDevice.h"
#include "platform/opengl/SoloOpenGLGraphics.h"
#include "platform/stub/SoloStubGraphics.h"

using namespace solo;


shared<Graphics> Graphics::create(Device* device)
{
	if (device->getMode() == DeviceMode::OpenGL)
		return SL_NEW_SHARED(OpenGLGraphics, device);
	return SL_NEW_SHARED(StubGraphics, device);
}


Graphics::Graphics(Device *device):
	device(device)
{
}
