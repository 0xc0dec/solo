#pragma once

#include "SoloBase.h"
#include "SoloComponent.h"
#include "SoloNode.h"

namespace solo
{
    enum class DeviceMode;
    class Material;
    class Mesh2;
    class MeshEffectBinding;

    class MeshRenderer: public ComponentBase<MeshRenderer>
    {
    public:
        static shared<MeshRenderer> create(DeviceMode mode, Node node);

        virtual void render(RenderContext &context) override;

        Mesh2 *getMesh() const;
        void setMesh(shared<Mesh2> mesh);

        Material *findMaterial(unsigned index) const;
        size_t getMaterialCount() const;

        void setMaterial(unsigned index, shared<Material> material);

    private:
        MeshRenderer(DeviceMode mode, Node node);

        shared<Mesh2> mesh;
        DeviceMode deviceMode;
        std::unordered_map<unsigned, shared<Material>> materials;
        std::unordered_map<unsigned, shared<MeshEffectBinding>> bindings;
    };

    inline Mesh2 *MeshRenderer::getMesh() const
    {
        return mesh.get();
    }

    inline size_t MeshRenderer::getMaterialCount() const
    {
        return materials.size();
    }
}