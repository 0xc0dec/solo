#include "SoloDevice.h"
#include "platform/stub/SoloStubRenderTarget.h"
#include "platform/opengl/SoloOpenGLRenderTarget.h"

using namespace solo;


shared<RenderTarget> RenderTarget::create(DeviceMode mode)
{
	if (mode == DeviceMode::OpenGL)
		return SL_NEW2(OpenGLRenderTarget);
	return SL_NEW2(StubRenderTarget);
}
