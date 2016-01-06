#include "SoloOpenglGraphics.h"
#include "SoloMaterial.h"
#include "SoloDevice.h"
#include "SoloResourceManager.h"
#include "SoloRenderTarget.h"
#include "SoloMesh.h"
#include "SoloRenderContext.h"
#include <GL/glew.h>

using namespace solo;


OpenGLGraphics::OpenGLGraphics(Device *device):
    Graphics(device)
{
}


void OpenGLGraphics::renderSurface(Material *material, RenderTarget *target)
{
    if (!quadMesh)
        quadMesh = device->getResourceManager()->findMesh(KnownUris::UnitQuadMesh);

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
    quadMesh->draw();
    material->unbind(ctx);

    material->setDepthTestEnabled(depthTestEnabled); // TODO really needed?

    if (target)
        target->unbind();
}
