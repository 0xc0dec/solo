#include "SoloSurfaceRenderer.h"
#include "SoloRenderer.h"
#include "SoloResourceManager.h"
#include "SoloMesh.h"
#include "SoloMaterial.h"
#include "SoloDevice.h"
#include "SoloFrameBuffer.h"
#include "SoloRenderContext.h"

using namespace solo;


SurfaceRenderer::SurfaceRenderer(Renderer* renderer, shared<Material> material):
    renderer(renderer),
    device(renderer->getDevice()),
    material(material)
{
    auto resourceManager = device->getResourceManager();
    mesh = resourceManager->getOrCreatePrefabMesh(MeshPrefab::Quad, "/solo/internal/surface-renderer/mesh");
    binding = mesh->createEffectBinding(material->getEffect());
}


SurfaceRenderer::~SurfaceRenderer()
{
    binding.destroy();
}


void SurfaceRenderer::renderSurface(FrameBuffer* target)
{
    auto depthTestEnabled = material->isDepthTestEnabled();
    material->setDepthTestEnabled(false);

    if (target)
        target->bind();

    auto viewportSize = target ? target->getSize() : device->getCanvasSize();
    renderer->setViewport(0, 0, viewportSize.x, viewportSize.y);

    RenderContext ctx;
    material->bind(ctx);
    mesh->drawIndex(0, &binding);
    material->unbind(ctx);

    material->setDepthTestEnabled(depthTestEnabled); // TODO really needed?

    if (target)
        target->unbind();
}
