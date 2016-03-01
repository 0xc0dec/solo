#include "SoloMeshRenderer.h"
#include "SoloMesh.h"
#include "SoloMaterial.h"
#include "SoloRenderContext.h"
#include "SoloMeshEffectBinding.h"
#include "SoloDevice.h"

using namespace solo;


MeshRenderer::MeshRenderer(Node node):
    ComponentBase(node),
    deviceMode(node.getScene()->getDevice()->getMode())
{
    renderQueue = KnownRenderQueues::OpaqueObjects;
}


void MeshRenderer::render(RenderContext& context)
{
    if (!mesh)
        return;

    auto indexCount = mesh->getIndexCount();
    if (indexCount == 0)
    {
        auto material = findMaterial(0);
        if (material)
        {
            material->bind(context);
            bindings[0]->bind();
            mesh->draw();
            bindings[0]->unbind();
            material->unbind(context);
        }
    }
    else
    {
        for (auto i = 0; i < indexCount; ++i)
        {
            auto material = findMaterial(i);
            if (material)
            {
                material->bind(context);
                bindings[i]->bind();
                mesh->drawIndex(i);
                bindings[i]->unbind();
                material->unbind(context);
            }
        }
    }
}


void MeshRenderer::setMesh(shared<Mesh> mesh)
{
    this->mesh = mesh;
    // TODO maybe retain one material, if any
    materials.clear();
    bindings.clear();
}


void MeshRenderer::setMaterial(int index, shared<Material> material)
{
    if (!mesh)
        SL_THROW_FMT(InvalidOperationException, "Renderer has no mesh, setting material has no effect");

    auto indexCount = mesh->getIndexCount();
    if (indexCount > 0 && index >= indexCount)
        SL_THROW_FMT(InvalidOperationException, "Trying to set material with index ", index, ", but mesh has only ", indexCount, " indexes");

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


Material* MeshRenderer::findMaterial(int index) const
{
    if (materials.find(index) == materials.end())
        return nullptr;
    return materials.at(index).get();
}
