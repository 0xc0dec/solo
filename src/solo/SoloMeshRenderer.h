/*
    Copyright (c) Aleksey Fedotov
    MIT license
*/

#pragma once

#include "SoloCommon.h"
#include "SoloComponent.h"
#include "SoloNode.h"
#include "SoloMap.h"

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

        auto getMaterial(uint32_t index) const -> Material* { return materials.at(index).get(); }
        void setMaterial(uint32_t index, sptr<Material> material);
        auto getMaterialCount() const -> uint32_t { return static_cast<uint32_t>(materials.size()); }

    private:
        sptr<Mesh> mesh;
        Transform *transform = nullptr;
        Renderer *renderer = nullptr;
        umap<uint32_t, sptr<Material>> materials;
    };
}