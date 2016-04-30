#include "SoloRenderer.h"
#include "SoloDevice.h"
#include "platform/opengl/SoloOpenGLRenderer.h"
#include "platform/stub/SoloStubRenderer.h"

using namespace solo;


auto Renderer::create(Device* device, const DeviceToken&) -> uptr<Renderer>
{
    if (device->getMode() == DeviceMode::OpenGL)
        return std::unique_ptr<OpenGLRenderer>(new OpenGLRenderer());
    return std::unique_ptr<StubRenderer>(new StubRenderer());
}
