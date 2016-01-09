#include "SoloOpenGLSurfaceRenderer.h"
#include "SoloDevice.h"
#include "SoloResourceManager.h"
#include "SoloMaterial.h"
#include "SoloMesh.h"
#include "SoloRenderTarget.h"
#include "SoloRenderContext.h"
#include "../../SoloMeshEffectBinding.h"
#include <GL/glew.h>

using namespace solo;


OpenGLSurfaceRenderer::OpenGLSurfaceRenderer(Device *device, shared<Material> material):
    device(device),
    material(material)
{
    auto resourceManager = device->getResourceManager();
    mesh = resourceManager->getOrCreatePrefabMesh(MeshPrefab::Quad, "/solo/internal/surface-renderer/mesh");
    binding = MeshEffectBinding::create(device->getMode(), mesh.get(), material->getEffect());
}


void OpenGLSurfaceRenderer::renderSurface(RenderTarget* target)
{
    auto depthTestEnabled = material->isDepthTestEnabled();
    material->setDepthTestEnabled(false);

    if (target)
        target->bind();

    auto viewportSize = target ? target->getColorAttachmentSize() : device->getCanvasSize();
    glViewport(
        static_cast<GLuint>(0),
        static_cast<GLuint>(0),
        static_cast<GLuint>(viewportSize.x),
        static_cast<GLuint>(viewportSize.y));

    RenderContext ctx;
    material->bind(ctx);
    binding->bind();
    mesh->drawPart(0);
    binding->unbind();
    material->unbind(ctx);

    material->setDepthTestEnabled(depthTestEnabled); // TODO really needed?

    if (target)
        target->unbind();
}