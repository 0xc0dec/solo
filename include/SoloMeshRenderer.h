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

        Material* getMaterial(int index) const;
        void setMaterial(int index, shared<Material> material);
        int getMaterialCount() const;

    private:
        shared<Mesh> mesh;
        DeviceMode deviceMode;
        std::unordered_map<int, shared<Material>> materials;
    };

    inline Mesh* MeshRenderer::getMesh() const
    {
        return mesh.get();
    }

    inline void MeshRenderer::setMesh(shared<Mesh> mesh)
    {
        this->mesh = mesh;
    }

    inline Material* MeshRenderer::getMaterial(int index) const
    {
        return materials.at(index).get();
    }

    inline int MeshRenderer::getMaterialCount() const
    {
        return static_cast<int>(materials.size());
    }
}