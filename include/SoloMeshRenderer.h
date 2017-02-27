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

        auto getMesh() const -> Mesh*;
        void setMesh(sptr<Mesh> mesh);

        auto getMaterial(uint32_t index) const -> Material*;
        void setMaterial(uint32_t index, sptr<Material> material);
        auto getMaterialCount() const -> uint32_t;

    private:
        sptr<Mesh> mesh;
        Transform *transform = nullptr;
        Renderer *renderer = nullptr;
        std::unordered_map<uint32_t, sptr<Material>> materials;
    };

    inline auto MeshRenderer::getMesh() const -> Mesh *
    {
        return mesh.get();
    }

    inline void MeshRenderer::setMesh(sptr<Mesh> mesh)
    {
        this->mesh = mesh;
    }

    inline auto MeshRenderer::getMaterial(uint32_t index) const -> Material*
    {
        return materials.at(index).get();
    }

    inline auto MeshRenderer::getMaterialCount() const -> uint32_t
    {
        return static_cast<uint32_t>(materials.size());
    }
}