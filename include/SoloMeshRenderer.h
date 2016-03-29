#pragma once

#include "SoloBase.h"
#include "SoloComponent.h"
#include "SoloNode.h"


namespace solo
{
    class Material;
    class Mesh;

    class MeshRenderer final: public ComponentBase<MeshRenderer>
    {
    public:
        explicit MeshRenderer(Node node);

        virtual void render(RenderContext& context) override final;

        Mesh* getMesh() const;
        void setMesh(sptr<Mesh> mesh);

        Material* getMaterial(uint32_t index) const;
        void setMaterial(uint32_t index, sptr<Material> material);
        uint32_t getMaterialCount() const;

    private:
        sptr<Mesh> mesh;
        std::unordered_map<uint32_t, sptr<Material>> materials;
    };

    inline Mesh* MeshRenderer::getMesh() const
    {
        return mesh.get();
    }

    inline void MeshRenderer::setMesh(sptr<Mesh> mesh)
    {
        this->mesh = mesh;
    }

    inline Material* MeshRenderer::getMaterial(uint32_t index) const
    {
        return materials.at(index).get();
    }

    inline uint32_t MeshRenderer::getMaterialCount() const
    {
        return static_cast<uint32_t>(materials.size());
    }
}