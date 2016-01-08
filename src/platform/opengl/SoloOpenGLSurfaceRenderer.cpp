#include "SoloOpenGLSurfaceRenderer.h"
#include "SoloDevice.h"
#include "SoloResourceManager.h"
#include "SoloMaterial.h"
#include "../../SoloMeshEffectBinding.h"

using namespace solo;


OpenGLSurfaceRenderer::OpenGLSurfaceRenderer(Device *device, Material *material):
    device(device)
{
    quadMesh = device->getResourceManager()->findMesh(KnownUris::UnitQuadMesh);
    binding = MeshEffectBinding::create(device->getMode(), quadMesh.get(), material->getEffect());
}


void OpenGLSurfaceRenderer::renderSurface(Material* material, RenderTarget* target)
{
}