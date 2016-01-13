#include "SoloSurfaceRenderer.h"
#include "SoloDevice.h"
#include "platform/opengl/SoloOpenGLSurfaceRenderer.h"
#include "platform/stub/SoloStubSurfaceRenderer.h"

using namespace solo;


shared<SurfaceRenderer> SurfaceRenderer::create(Device* device, shared<Material> material)
{
    if (device->getMode() == DeviceMode::OpenGL)
        return SL_NEW_SHARED(OpenGLSurfaceRenderer, device, material);
    return SL_NEW_SHARED(StubSurfaceRenderer);
}
