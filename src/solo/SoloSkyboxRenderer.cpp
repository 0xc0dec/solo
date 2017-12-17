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

    material = Material::createFromPrefab(device, MaterialPrefab::Skybox);
    material->setDepthTest(true);
    material->setDepthWrite(false);
    material->setFaceCull(FaceCull::None);

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
