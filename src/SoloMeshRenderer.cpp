#include "SoloMeshRenderer.h"
#include "SoloMesh.h"
#include "SoloMaterial.h"
#include "SoloRenderContext.h"
#include "SoloMeshEffectBinding.h"
#include "SoloDevice.h"

using namespace solo;


shared<MeshRenderer> MeshRenderer::create(Node node)
{
    return SL_NEW_SHARED(MeshRenderer, node);
}


MeshRenderer::MeshRenderer(Node node):
    ComponentBase(node),
    deviceMode(node.getScene()->getDevice()->getMode())
{
    renderQueue = KnownRenderQueues::OpaqueObjects;
}


void MeshRenderer::render(RenderContext &context)
{
    auto partCount = mesh->getPartCount();
    if (partCount == 0)
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
        for (unsigned i = 0; i < partCount; ++i)
        {
            auto material = findMaterial(i);
            if (material)
            {
                material->bind(context);
                bindings[i]->bind();
                mesh->drawPart(i);
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


void MeshRenderer::setMaterial(unsigned index, shared<Material> material)
{
    if (!mesh)
        SL_THROW_FMT(EngineException, "Renderer has no mesh, setting material has no effect");

    auto partCount = mesh->getPartCount();
    if (partCount > 0 && index >= partCount)
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


Material *MeshRenderer::findMaterial(unsigned index) const
{
    if (materials.find(index) == materials.end())
        return nullptr;
    return materials.at(index).get();
}
