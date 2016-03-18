#include "SoloMeshRenderer.h"
#include "SoloMesh.h"
#include "SoloMaterial.h"
#include "SoloRenderContext.h"
#include "SoloDevice.h"

using namespace solo;


MeshRenderer::MeshRenderer(Node node):
    ComponentBase(node),
    deviceMode(node.getScene()->getDevice()->getMode())
{
    renderQueue = KnownRenderQueues::OpaqueObjects;
}


void MeshRenderer::render(RenderContext& context)
{
    if (!mesh || materials.empty())
        return;

    auto partCount = mesh->getPartCount();
    if (partCount == 0)
    {
        auto material = getMaterial(0);
        if (material)
        {
            material->bind(context);
            mesh->draw(material->getEffect());
        }
    }
    else
    {
        for (uint32_t part = 0; part < partCount; ++part)
        {
            auto material = getMaterial(part);
            if (material)
            {
                material->bind(context);
                mesh->drawPart(material->getEffect(), part);
            }
        }
    }
}


void MeshRenderer::setMaterial(uint32_t index, shared<Material> material)
{
    if (material)
        materials[index] = material;
    else
        materials.erase(index);
}
