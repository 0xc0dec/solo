/*
 * Copyright (c) Aleksey Fedotov
 * MIT license
 */

#pragma once

#include "SoloCommon.h"
#include "SoloComponent.h"
#include "SoloNode.h"

namespace solo {
    class Material;
    class Mesh;
    class Transform;
    class Renderer;

    class MeshRenderer final: public ComponentBase<MeshRenderer> {
    public:
        explicit MeshRenderer(const Node &node);

        void render() override;

        auto mesh() const -> sptr<Mesh> { return mesh_; }
        void setMesh(const sptr<Mesh> &mesh) {
            this->mesh_ = mesh;
        }

        auto material(u32 index) const -> sptr<Material>;
        auto materialCount() const -> u32 {
            return materialCount_;
        }
        void setMaterial(u32 index, const sptr<Material> &material);
        void setDefaultMaterial(const sptr<Material> &material);

    private:
        sptr<Mesh> mesh_;
        Transform *transform_ = nullptr;
        Renderer *renderer_ = nullptr;
        sptr<Material> defaultMaterial_ = nullptr;
        vec<sptr<Material>> materials_;
        u32 materialCount_ = 0;
    };
}