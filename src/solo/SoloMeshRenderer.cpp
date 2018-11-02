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
    renderer_(node.scene()->device()->renderer())
{
    transform_ = node.findComponent<Transform>();
}

void MeshRenderer::render()
{
    if (!mesh_ || materials_.empty())
        return;

    const auto partCount = mesh_->partCount();
    if (partCount == 0)
    {
        const auto mat = material(0);
        if (mat)
            renderer_->drawMesh(mesh_.get(), transform_, mat.get());
    }
    else
    {
        for (u32 part = 0; part < partCount; ++part)
        {
            const auto mat = material(part);
            if (mat)
                renderer_->drawMeshPart(mesh_.get(), part, transform_, mat.get());
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
