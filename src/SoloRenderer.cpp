#include "SoloRenderer.h"
#include "SoloDevice.h"
#include "platform/opengl/SoloOpenGLRenderer.h"
#include "platform/stub/SoloStubRenderer.h"

using namespace solo;


uptr<Renderer> Renderer::create(Device* device, const DeviceToken&)
{
    if (device->getMode() == DeviceMode::OpenGL)
        return std::unique_ptr<OpenGLRenderer>(new OpenGLRenderer());
    return std::unique_ptr<StubRenderer>(new StubRenderer());
}


Renderer::~Renderer()
{
}
