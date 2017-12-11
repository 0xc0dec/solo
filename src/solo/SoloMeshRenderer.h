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

        auto getMesh() const -> Mesh* { return mesh.get(); }
        void setMesh(sptr<Mesh> mesh) { this->mesh = mesh; }

        auto getMaterial(u32 index) const -> sptr<Material> { return materials.at(index); }
        void setMaterial(u32 index, sptr<Material> material);
        auto getMaterialCount() const -> u32 { return static_cast<u32>(materials.size()); }

    private:
        sptr<Mesh> mesh;
        Transform *transform = nullptr;
        Renderer *renderer = nullptr;
        umap<u32, sptr<Material>> materials;
    };
}