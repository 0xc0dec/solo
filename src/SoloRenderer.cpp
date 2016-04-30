#include "SoloRenderer.h"
#include "SoloDevice.h"
#include "platform/opengl/SoloOpenGLRenderer.h"
#include "platform/vulkan/SoloVulkanRenderer.h"
#include "platform/stub/SoloStubRenderer.h"

using namespace solo;


auto Renderer::create(Device* device, const DeviceToken&) -> uptr<Renderer>
{
    switch (device->getMode())
    {
        case DeviceMode::OpenGL:
            return std::unique_ptr<OpenGLRenderer>(new OpenGLRenderer());
        case DeviceMode::Vulkan:
            return std::unique_ptr<VulkanRenderer>(new VulkanRenderer());
        default:
            return std::unique_ptr<StubRenderer>(new StubRenderer());
    }
}
