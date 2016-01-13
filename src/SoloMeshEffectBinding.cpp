#include "SoloMeshEffectBinding.h"
#include "SoloDevice.h"
#include "platform/opengl/SoloOpenGLMeshEffectBinding.h"
#include "platform/stub/SoloStubMeshEffectBinding.h"

using namespace solo;


shared<MeshEffectBinding> MeshEffectBinding::create(DeviceMode mode, Mesh* mesh, Effect* effect)
{
    if (mode == DeviceMode::OpenGL)
        return SL_NEW_SHARED(OpenGLMeshEffectBinding, mesh, effect);
    return SL_NEW_SHARED(StubMeshEffectBinding, mesh, effect);
}
