#include "SoloRenderer.h"
#include "SoloDevice.h"
#include "platform/opengl/SoloOpenGLRenderer.h"
#include "platform/stub/SoloStubRenderer.h"

using namespace solo;


shared<Renderer> Renderer::create(DeviceMode mode)
{
    if (mode == DeviceMode::OpenGL)
        return SL_NEW_SHARED(OpenGLRenderer);
    return SL_NEW_SHARED(StubRenderer);
}


Renderer::Renderer()
{
}


Renderer::~Renderer()
{
}
