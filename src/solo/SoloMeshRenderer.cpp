/*
    Copyright (c) Aleksey Fedotov
    MIT license
*/

#include "SoloMeshRenderer.h"
#include "SoloMesh.h"
#include "SoloMaterial.h"
#include "SoloTransform.h"
#include "SoloDevice.h"

using namespace solo;

MeshRenderer::MeshRenderer(const Node &node):
    ComponentBase(node),
    renderer_(node.getScene()->getDevice()->getRenderer())
{
    transform_ = node.findComponent<Transform>();
}

void MeshRenderer::render()
{
    if (!mesh_ || materials_.empty())
        return;

    const auto partCount = mesh_->getPartCount();
    if (partCount == 0)
    {
        const auto material = getMaterial(0);
        if (material)
            renderer_->drawMesh(mesh_.get(), transform_, material.get());
    }
    else
    {
        for (u32 part = 0; part < partCount; ++part)
        {
            const auto material = getMaterial(part);
            if (material)
                renderer_->drawMeshPart(mesh_.get(), part, transform_, material.get());
        }
    }
}

void MeshRenderer::setMaterial(u32 index, sptr<Material> material)
{
    if (material)
        materials_[index] = material;
    else
        materials_.erase(index);
}
