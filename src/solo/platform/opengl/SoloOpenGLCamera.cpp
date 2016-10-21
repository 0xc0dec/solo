#include "SoloOpenGLCamera.h"
#include "SoloDevice.h"
#include "SoloOpenGLRenderer.h"

using namespace solo;


OpenGLCamera::OpenGLCamera(const Node& node):
    Camera(node)
{
    renderer = dynamic_cast<OpenGLRenderer*>(Device::get()->getRenderer());
}


void OpenGLCamera::applyImpl() const
{
    if (viewportSet)
    {
        renderer->setViewport(
            static_cast<uint32_t>(viewport.x),
            static_cast<uint32_t>(viewport.y),
            static_cast<uint32_t>(viewport.z),
            static_cast<uint32_t>(viewport.w));
    }
    else
    {
        auto size = device->getCanvasSize();
        renderer->setViewport(0, 0, static_cast<uint32_t>(size.x), static_cast<uint32_t>(size.y));
    }

    renderer->setDepthWrite(true);
    renderer->setDepthTest(true);
    renderer->clear(true, true, clearColor.x, clearColor.y, clearColor.z, clearColor.w);
}
