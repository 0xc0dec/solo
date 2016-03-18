#include "SoloGraphics.h"
#include "SoloDevice.h"
#include "SoloResourceManager.h"
#include "SoloMesh.h"
#include "SoloMaterial.h"
#include "SoloFrameBuffer.h"
#include "SoloDevice.h"
#include "SoloRenderContext.h"

using namespace solo;


Graphics::Graphics(Device* device):
    device(device),
    renderer(device->getRenderer())
{
}


void Graphics::blit(Material* material, FrameBuffer* target)
{
    if (!quadMesh)
        quadMesh = device->getResourceManager()->getOrCreatePrefabMesh(MeshPrefab::Quad, "/solo/internal/graphics/quad-mesh");

    material->setDepthTest(false);
    material->setDepthWrite(false);
    material->setPolygonFace(PolygonFace::All);

    if (target)
        target->bind();

    auto viewportSize = target ? target->getSize() : device->getCanvasSize();
    renderer->setViewport(0, 0, viewportSize.x, viewportSize.y);

    RenderContext ctx;
    material->bind(ctx);
    quadMesh->draw(material->getEffect());

    if (target)
        target->unbind();
}
