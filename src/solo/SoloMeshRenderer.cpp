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

sptr<Mesh> MeshRenderer::errorMesh_ = nullptr;

MeshRenderer::MeshRenderer(const Node &node):
    ComponentBase(node),
    renderer_(node.scene()->device()->renderer()) {
    transform_ = node.findComponent<Transform>();

    if (!errorMesh_) {
        errorMesh_ = Mesh::empty(node.scene()->device());
        auto layout = VertexBufferLayout();
        layout.addAttribute(VertexAttributeUsage::Position);

        errorMesh_->addVertexBuffer(layout, {
            // front
            -1.0, -1.0,  1.0,
             1.0, -1.0,  1.0,
             1.0,  1.0,  1.0,
            -1.0,  1.0,  1.0,
            // back
            -1.0, -1.0, -1.0,
             1.0, -1.0, -1.0,
             1.0,  1.0, -1.0,
            -1.0,  1.0, -1.0
        }, 8);
        errorMesh_->addIndexBuffer({
            // front
		    0, 1, 2,
		    2, 3, 0,
		    // right
		    1, 5, 6,
		    6, 2, 1,
		    // back
		    7, 6, 5,
		    5, 4, 7,
		    // left
		    4, 0, 3,
		    3, 7, 4,
		    // bottom
		    4, 5, 1,
		    1, 0, 4,
		    // top
		    3, 2, 6,
		    6, 7, 3
        }, 36);
        errorMesh_->setPrimitiveType(PrimitiveType::Triangles);
    }
}

auto MeshRenderer::material(u32 index) const -> sptr<Material> {
    auto mat = index < materials_.size() ? materials_[index] : nullptr;
    return mat ? mat : defaultMaterial_;
}

void MeshRenderer::render() {
    const auto mesh = mesh_ ? mesh_ : errorMesh_;

    const auto indexCount = mesh->indexBufferCount();
    if (!indexCount) {
        const auto mat = material(0);
        renderer_->renderMesh(mesh.get(), transform_, mat.get());
    } else {
        for (u32 index = 0; index < indexCount; ++index) {
            const auto mat = material(index);
            renderer_->renderMeshIndex(mesh.get(), index, transform_, mat.get());
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
