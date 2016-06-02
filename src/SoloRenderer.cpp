#include "SoloRenderer.h"
#include "SoloDevice.h"
#include "platform/opengl/SoloOpenGLRenderer.h"
#include "platform/vulkan/SoloVulkanRenderer.h"
#include "platform/stub/SoloStubRenderer.h"

using namespace solo;


auto Renderer::create(Device* device, const DeviceToken&) -> sptr<Renderer>
{
    switch (device->getMode())
    {
        case DeviceMode::OpenGL:
            return std::make_shared<OpenGLRenderer>();
#if 0
        case DeviceMode::Vulkan:
            return std::make_shared<VulkanRenderer>();
#endif
        default:
            return std::make_shared<StubRenderer>();
    }
}
