/*
    Copyright (c) Aleksey Fedotov
    MIT license
*/

#include "SoloSkyboxRenderer.h"
#include "SoloMesh.h"
#include "SoloMaterial.h"
#include "SoloTexture.h"
#include "SoloTransform.h"
#include "SoloDevice.h"
#include "SoloRenderCommand.h"

using namespace solo;

SkyboxRenderer::SkyboxRenderer(const Node &node):
    ComponentBase(node),
    renderer(node.getScene()->getDevice()->getRenderer())
{
    transform = node.findComponent<Transform>();

    const auto device = node.getScene()->getDevice();

    const auto effect = Effect::createFromPrefab(device, EffectPrefab::Skybox);
    material = Material::create(device, effect);
    material->setDepthTest(true);
    material->setDepthWrite(false);
    material->setFaceCull(FaceCull::None);

    // TODO avoid this, add prefab materials and do dirty stuff there
    const auto deviceMode = node.getScene()->getDevice()->getMode();
    switch (deviceMode)
    {
        case DeviceMode::OpenGL:
            material->bindParameter("projMatrix", BindParameterSemantics::ProjectionMatrix);
            material->bindParameter("worldViewMatrix", BindParameterSemantics::WorldViewMatrix);
            break;
        case DeviceMode::Vulkan:
            material->bindParameter("matrices.proj", BindParameterSemantics::ProjectionMatrix);
            material->bindParameter("matrices.worldView", BindParameterSemantics::WorldViewMatrix);
            break;
        default:
            break;
    }

    quadMesh = Mesh::createFromPrefab(device, MeshPrefab::Quad);
}

void SkyboxRenderer::render()
{
    renderer->addRenderCommand(RenderCommand::drawMesh(quadMesh.get(), transform, material.get())); // TODO transform needed here?
}

void SkyboxRenderer::setTexture(sptr<CubeTexture> texture)
{
    this->texture = texture;
    material->setTextureParameter("mainTex", texture);
}
