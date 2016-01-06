#pragma once

#include "SoloBase.h"
#include "SoloComponent.h"
#include "SoloNode.h"

namespace solo
{
    class Model;
    class Material;

    class ModelRenderer: public ComponentBase<ModelRenderer>
    {
    public:
        static shared<ModelRenderer> create(Node node);

        virtual void render(RenderContext &context) override;

        Model *getModel() const;
        void setModel(shared<Model> model);

        Material *getMaterial(unsigned index) const;
        size_t getMaterialCount() const;

        void setMaterialForMesh(unsigned index, shared<Material> material);
        void setMaterial(shared<Material> material);

    private:
        explicit ModelRenderer(Node node);

        shared<Model> model;
        std::unordered_map<unsigned, shared<Material>> materials;
    };
}