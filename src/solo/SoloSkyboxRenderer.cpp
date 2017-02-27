/*
    Copyright (c) Aleksey Fedotov
    MIT license
*/

#include "SoloSkyboxRenderer.h"
#include "SoloMesh.h"
#include "SoloMaterial.h"
#include "SoloCubeTexture.h"
#include "SoloTransform.h"
#include "SoloDevice.h"

using namespace solo;


SkyboxRenderer::SkyboxRenderer(const Node &node):
    ComponentBase(node),
    renderer(node.getScene()->getDevice()->getRenderer())
{
    transform = node.findComponent<Transform>();

    auto device = node.getScene()->getDevice();

    auto effect = Effect::createFromPrefab(device, EffectPrefab::Skybox);
    material = Material::create(device, effect);
    material->bindProjectionMatrixParameter("projMatrix");
    material->bindWorldViewMatrixParameter("worldViewMatrix");
    material->setDepthTest(true);
    material->setDepthWrite(false);
    material->setFaceCull(FaceCull::CW);

    quadMesh = Mesh::createFromPrefab(device, MeshPrefab::Quad);
}


void SkyboxRenderer::render()
{
    renderer->addRenderCommand(RenderCommand::applyMaterial(material.get()));
    renderer->addRenderCommand(RenderCommand::drawMesh(quadMesh.get(), transform)); // TODO transform needed here?
}


void SkyboxRenderer::setTexture(sptr<CubeTexture> texture)
{
    this->texture = texture;
    material->setTextureParameter("mainTex", texture);
}
