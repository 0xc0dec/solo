#include "SoloMeshEffectBinding.h"
#include "SoloDevice.h"
#include "platform/opengl/SoloOpenGLMeshEffectBinding.h"

using namespace solo;


shared<MeshEffectBinding> MeshEffectBinding::create(DeviceMode mode, Mesh* mesh, Effect* effect)
{
    // TODO take mode into account
    return SL_NEW_SHARED(OpenGLMeshEffectBinding, mesh, effect);
}
