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

    class MeshRenderer: public ComponentBase<MeshRenderer>
    {
    public:
        static shared<MeshRenderer> create(DeviceMode mode, Node node);

        virtual void render(RenderContext &context) override;

        Mesh *getMesh() const;
        void setMesh(shared<Mesh> mesh);

        Material *findMaterial(unsigned index) const;
        size_t getMaterialCount() const;

        void setMaterial(unsigned index, shared<Material> material);

    private:
        MeshRenderer(DeviceMode mode, Node node);

        shared<Mesh> mesh;
        DeviceMode deviceMode;
        std::unordered_map<unsigned, shared<Material>> materials;
        std::unordered_map<unsigned, shared<MeshEffectBinding>> bindings;
    };

    inline Mesh *MeshRenderer::getMesh() const
    {
        return mesh.get();
    }

    inline size_t MeshRenderer::getMaterialCount() const
    {
        return materials.size();
    }
}