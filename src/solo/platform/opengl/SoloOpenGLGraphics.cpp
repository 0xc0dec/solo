#include "SoloOpenGLGraphics.h"
#include "SoloRenderContext.h"
#include "SoloMesh.h"
#include "SoloMaterial.h"
#include "SoloFrameBuffer.h"
#include "SoloDevice.h"
#include "platform/opengl/SoloOpenGLRenderer.h"

using namespace solo;


OpenGLGraphics::OpenGLGraphics(Device* device, const DeviceToken& token):
    Graphics(token),
    device(device)
{
    renderer = dynamic_cast<OpenGLRenderer*>(device->getRenderer());
}


void OpenGLGraphics::blit(Material* material, FrameBuffer* target)
{
    if (!quadMesh)
        quadMesh = Mesh::create(MeshPrefab::Quad);

    material->setDepthTest(false);
    material->setDepthWrite(false);
    material->setFaceCull(FaceCull::All);

    if (target)
        target->bind();

    auto viewportSize = target ? target->getSize() : device->getCanvasSize();
    renderer->setViewport(static_cast<uint32_t>(0),
                          static_cast<uint32_t>(0),
                          static_cast<uint32_t>(viewportSize.x),
                          static_cast<uint32_t>(viewportSize.y));

    material->apply(RenderContext::empty);
    quadMesh->draw(material->getEffect());

    if (target)
        target->unbind();
}
