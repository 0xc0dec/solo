#include "SoloMeshRenderer.h"
#include "SoloMesh.h"
#include "SoloMaterial.h"
#include "SoloRenderContext.h"
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
            mesh->draw(&bindings[0]);
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
                mesh->drawIndex(i, &bindings[i]);
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
    while (!bindings.empty())
    {
        bindings.begin()->second.destroy();
        bindings.erase(bindings.begin());
    }
}


void MeshRenderer::setMaterial(int index, shared<Material> material)
{
    if (material)
    {
        materials[index] = material;
        bindings[index] = mesh->createEffectBinding(material->getEffect());
    }
    else
    {
        materials.erase(index);
        bindings[index].destroy();
        bindings.erase(index);
    }
}


Material* MeshRenderer::findMaterial(int index) const
{
    if (materials.find(index) == materials.end())
        return nullptr;
    return materials.at(index).get();
}
