/*
    Copyright (c) Aleksey Fedotov
    MIT license
*/

#include "SoloMeshRenderer.h"
#include "SoloMesh.h"
#include "SoloMaterial.h"
#include "SoloTransform.h"
#include "SoloDevice.h"
#include "SoloRenderCommand.h"

using namespace solo;

MeshRenderer::MeshRenderer(const Node &node):
    ComponentBase(node),
    renderer(node.getScene()->getDevice()->getRenderer())
{
    transform = node.findComponent<Transform>();
}

void MeshRenderer::render()
{
    if (!mesh || materials.empty())
        return;

    const auto partCount = mesh->getPartCount();
    if (partCount == 0)
    {
        const auto material = getMaterial(0);
        if (material)
            renderer->addRenderCommand(RenderCommand::drawMesh(mesh.get(), transform, material));
    }
    else
    {
        for (uint32_t part = 0; part < partCount; ++part)
        {
            const auto material = getMaterial(part);
            if (material)
                renderer->addRenderCommand(RenderCommand::drawMeshPart(mesh.get(), part, transform, material));
        }
    }
}

void MeshRenderer::setMaterial(uint32_t index, sptr<Material> material)
{
    if (material)
        materials[index] = material;
    else
        materials.erase(index);
}
