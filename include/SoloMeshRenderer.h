#pragma once

#include "SoloBase.h"
#include "SoloComponent.h"
#include "SoloNode.h"


namespace solo
{
    enum class DeviceMode;
    class Material;
    class Mesh;
    class MeshEffectBinding;

    class MeshRenderer final: public ComponentBase<MeshRenderer>
    {
    public:
        explicit MeshRenderer(Node node);

        virtual void render(RenderContext& context) override final;

        Mesh* getMesh() const;
        void setMesh(shared<Mesh> mesh);

        Material* findMaterial(int index) const;
        int getMaterialCount() const;

        void setMaterial(int index, shared<Material> material);

    private:
        shared<Mesh> mesh;
        DeviceMode deviceMode;
        std::unordered_map<int, shared<Material>> materials;
        std::unordered_map<int, shared<MeshEffectBinding>> bindings;
    };

    inline Mesh* MeshRenderer::getMesh() const
    {
        return mesh.get();
    }

    inline int MeshRenderer::getMaterialCount() const
    {
        return static_cast<int>(materials.size());
    }
}