#include "SoloModelRenderer.h"
#include "SoloModel.h"
#include "SoloMaterial.h"
#include "SoloMesh.h"
#include "SoloRenderContext.h"
#include "SoloMeshEffectBinding.h"

using namespace solo;


shared<MeshRenderer> MeshRenderer::create(DeviceMode mode, Node node)
{
    return SL_NEW_SHARED(MeshRenderer, mode, node);
}


void MeshRenderer::render(RenderContext& context)
{
    auto partCount = mesh->getPartCount();
    if (partCount == 0)
    {
        auto material = getMaterial(0);
        if (material)
        {
            material->bind(context);
            mesh->draw();
            material->unbind(context);
        }
    }
    else
    {
        for (unsigned i = 0; i < partCount; ++i)
        {
            auto material = getMaterial(i);
            if (material)
            {
                material->bind(context);
                mesh->drawIndexedPart(i);
                material->unbind(context);
            }
        }
    }
}


void MeshRenderer::setMaterialForMeshPart(unsigned index, shared<Material> material)
{
    if (!mesh)
        SL_THROW_FMT(EngineException, "Renderer has no mesh, setting material has no effect");
    if (index >= mesh->getPartCount())
        SL_THROW_FMT(EngineException, "Trying to set material with index ", index, ", but mesh has only ", mesh->getPartCount(), " parts");

    if (!material)
    {
        materials.erase(index);
        bindings.erase(index);
    }
    else
    {
        materials[index] = material;
        bindings[index] = MeshEffectBinding::create(deviceMode, mesh.get(), material->getEffect());
    }
}


void MeshRenderer::setMaterial(shared<Material> material)
{
    bindings.clear();
    materials.clear();

    if (!material)
        return;

    auto partCount = mesh->getPartCount();
    if (partCount)
    {
        for (size_t i = 0; i < partCount; ++i)
            setMaterialForMeshPart(i, material);
    }
    else
    {
        // TODO ugly
        materials[0] = material;
        bindings[0] = MeshEffectBinding::create(deviceMode, mesh.get(), material->getEffect());
    }
}


MeshRenderer::MeshRenderer(DeviceMode mode, Node node):
    ComponentBase(node),
    deviceMode(mode)
{
    renderQueue = KnownRenderQueues::OpaqueObjects;
}


Material *MeshRenderer::getMaterial(unsigned index) const
{
    if (materials.find(index) == materials.end())
        return nullptr;
    return materials.at(index).get();
}


shared<ModelRenderer> ModelRenderer::create(Node node)
{
    return SL_NEW_SHARED(ModelRenderer, node);
}


ModelRenderer::ModelRenderer(Node node):
    ComponentBase(node)
{
    renderQueue = KnownRenderQueues::OpaqueObjects;
}


void ModelRenderer::render(RenderContext &context)
{
    auto meshCount = model->getMeshCount();
    for (auto i = 0; i < meshCount; ++i)
    {
        auto material = getMaterial(i);
        if (material)
        {
            auto mesh = model->getMesh(i);
            material->bind(context);
            mesh->draw();
            material->unbind(context);
        }
    }
}


void ModelRenderer::setMaterialForMesh(unsigned index, shared<Material> material)
{
    if (!model)
        SL_THROW_FMT(EngineException, "Renderer has no model, hence setting material has no effect");
    if (index >= model->getMeshCount())
        SL_THROW_FMT(EngineException, "Trying to set material with index ", index, ", but model has only ", model->getMeshCount(), " meshes");
    if (!material)
        materials.erase(index);
    else
        materials[index] = material;
}


void ModelRenderer::setMaterial(shared<Material> material)
{
    if (!material)
        materials.clear();
    else
    {
        auto meshCount = model->getMeshCount();
        for (auto i = 0; i < meshCount; ++i)
            setMaterialForMesh(i, material);
    }
}


Material *ModelRenderer::getMaterial(unsigned index) const
{
    if (materials.find(index) == materials.end())
        return nullptr;
    return materials.at(index).get();
}
