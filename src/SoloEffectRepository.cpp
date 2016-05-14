#include "SoloEffectRepository.h"
#include "platform/opengl/SoloOpenGLEffectRepository.h"

using namespace solo;


auto EffectRepository::create(const DeviceToken&) -> sptr<EffectRepository>
{
    // No stub or other impls to far
    return std::make_shared<OpenGLEffectRepository>();
}
