/*
    Copyright (c) Aleksey Fedotov
    MIT license
*/

#pragma once

#include "SoloCommon.h"
#include "SoloComponent.h"
#include "SoloNode.h"

namespace solo
{
    class Material;
    class Mesh;
    class Transform;
    class Renderer;

    class MeshRenderer final: public ComponentBase<MeshRenderer>
    {
    public:
        explicit MeshRenderer(const Node &node);

        void render() override final;

        auto mesh() const -> sptr<Mesh> { return mesh_; }
        void setMesh(sptr<Mesh> mesh) { this->mesh_ = mesh; }

        auto material(u32 index) const -> sptr<Material> { return materials_.at(index); }
        auto materialCount() const -> u32 { return static_cast<u32>(materials_.size()); }
        void setMaterial(u32 index, sptr<Material> material);

    private:
        sptr<Mesh> mesh_;
        Transform *transform_ = nullptr;
        Renderer *renderer_ = nullptr;
        umap<u32, sptr<Material>> materials_;
    };
}