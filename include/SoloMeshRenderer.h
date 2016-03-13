#pragma once

#include "SoloBase.h"
#include "SoloComponent.h"
#include "SoloNode.h"


namespace solo
{
    enum class DeviceMode;
    class Material;
    class Mesh;

    class MeshRenderer final: public ComponentBase<MeshRenderer>
    {
    public:
        explicit MeshRenderer(Node node);

        virtual void render(RenderContext& context) override final;

        Mesh* getMesh() const;
        void setMesh(shared<Mesh> mesh);

        Material* getMaterial(uint32_t index) const;
        void setMaterial(uint32_t index, shared<Material> material);
        uint32_t getMaterialCount() const;

    private:
        shared<Mesh> mesh;
        DeviceMode deviceMode;
        std::unordered_map<uint32_t, shared<Material>> materials;
    };

    inline Mesh* MeshRenderer::getMesh() const
    {
        return mesh.get();
    }

    inline void MeshRenderer::setMesh(shared<Mesh> mesh)
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