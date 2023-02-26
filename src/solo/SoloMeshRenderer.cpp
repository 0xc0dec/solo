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

// TODO Move into separate file
static const auto FALLBACK_EFFECT_SRC = R"(
{
    vertex = {
        uniformBuffers = {
            matrices = {
                wvp = "mat4"
            }
        },

        inputs = {
            sl_Position = "vec4"
        },

        outputs = {
        },

        code = [[
            void main()
            {
                gl_Position = #matrices:wvp# * sl_Position;
                SL_FIX_Y#gl_Position#;
            }
        ]]
    },

    fragment = {
        samplers = {
        },

        outputs = {
            fragColor = { type = "vec4", target = 0 }
        },

        code = [[
            void main()
            {
                fragColor = vec4(1, 0, 1, 1);
            }
        ]]
    }
})";

sptr<Mesh> MeshRenderer::fallbackMesh_ = nullptr;
sptr<Material> MeshRenderer::fallbackMaterial_ = nullptr;

MeshRenderer::MeshRenderer(const Node &node):
    ComponentBase(node),
    renderer_(node.scene()->device()->renderer()) {
    const auto device = node.scene()->device();

    if (!fallbackMaterial_) {
        const auto errorEffect = Effect::fromDescription(device, FALLBACK_EFFECT_SRC);
        fallbackMaterial_ = Material::fromEffect(device, errorEffect);
        fallbackMaterial_->bindParameter("matrices:wvp", ParameterBinding::WorldViewProjectionMatrix);
    }

    if (!fallbackMesh_) {
        fallbackMesh_ = Mesh::empty(device);
        auto layout = VertexBufferLayout();
        layout.addAttribute(VertexAttributeUsage::Position);

        fallbackMesh_->addVertexBuffer(layout, {
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
        fallbackMesh_->addIndexBuffer({
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
        fallbackMesh_->setPrimitiveType(PrimitiveType::Triangles);
    }

    transform_ = node.findComponent<Transform>();
}

auto MeshRenderer::material(u32 index) const -> sptr<Material> {
    auto mat = index < materials_.size() ? materials_[index] : nullptr;
    return mat ? mat : (defaultMaterial_ ? defaultMaterial_ : fallbackMaterial_);
}

void MeshRenderer::render() {
    const auto mesh = mesh_ ? mesh_ : fallbackMesh_;

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
