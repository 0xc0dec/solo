/*
 * Copyright (c) Aleksey Fedotov
 * MIT license
 */

#include "SoloMeshRenderer.h"
#include "SoloMesh.h"
#include "SoloMaterial.h"
#include "SoloTransform.h"
#include "SoloDevice.h"

using namespace solo;

MeshRenderer::MeshRenderer(const Node &node):
    ComponentBase(node),
    renderer_(node.scene()->device()->renderer()) {
    transform_ = node.findComponent<Transform>();
}

auto MeshRenderer::material(u32 index) const -> sptr<Material> {
    auto mat = index < materials_.size() ? materials_[index] : nullptr;
    return mat ? mat : defaultMaterial_;
}

void MeshRenderer::render() {
    if (!mesh_)
        return;

    const auto indexCount = mesh_->indexBufferCount();
    if (!indexCount) {
        const auto mat = material(0);
        renderer_->renderMesh(mesh_.get(), transform_, mat.get());
    } else {
        for (u32 index = 0; index < indexCount; ++index) {
            const auto mat = material(index);
            renderer_->renderMeshIndex(mesh_.get(), index, transform_, mat.get());
        }
    }
}

void MeshRenderer::setMaterial(u32 index, const sptr<Material> &material) {
    if (index >= materials_.size())
        materials_.resize(index + 1);

    if (materials_[index] && !material)
        materialCount_--;
    else if (!materials_[index] && material)
        materialCount_++;

    materials_[index] = material;

    // TODO compaction when tail elements are all null
}

void MeshRenderer::setDefaultMaterial(const sptr<Material> &material) {
    defaultMaterial_ = material;
}
