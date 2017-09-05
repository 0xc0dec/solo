/*
    Copyright (c) Aleksey Fedotov
    MIT license
*/

#include "SoloFontRenderer.h"
#include "SoloMesh.h"
#include "SoloFont.h"
#include "SoloMaterial.h"
#include "SoloTransform.h"
#include "SoloDevice.h"
#include "SoloRectTexture.h"
#include "SoloRenderCommand.h"

using namespace solo;

FontRenderer::FontRenderer(const Node &node) :
    ComponentBase(node),
    renderer(node.getScene()->getDevice()->getRenderer())
{
    transform = node.findComponent<Transform>();

    const auto effect = Effect::createFromPrefab(node.getScene()->getDevice(), EffectPrefab::Font);
    material = Material::create(node.getScene()->getDevice(), effect);
    material->setFaceCull(FaceCull::All);
    material->bindWorldViewProjectionMatrixParameter("worldViewProjMatrix");
    material->setBlend(true);
    material->setDepthTest(true);
    material->setDepthWrite(false);
}

void FontRenderer::render()
{
    if (!mesh)
        return;
    renderer->addRenderCommand(RenderCommand::applyMaterial(material.get()));
    renderer->addRenderCommand(RenderCommand::drawMesh(mesh.get(), transform));
}

void FontRenderer::setFont(sptr<Font> newFont)
{
    if (newFont == font)
        return;

    font = newFont;

    if (font)
    {
        if (text.size() > 0)
            rebuildMesh();
        material->setTextureParameter("mainTex", font->getAtlas());
    }
    else
        mesh = nullptr;
}

void FontRenderer::setText(const std::string &newText)
{
    if (newText == text)
        return;

    auto oldLength = text.size();
    auto newLength = newText.size();
    text = newText;

    if (font && newLength > 0)
    {
        if (newLength == oldLength && mesh)
            updateMesh();
        else
            rebuildMesh();
    }
    else
        mesh = nullptr;
}

void FontRenderer::rebuildMesh()
{
    vertices.clear();
    uvs.clear();
    indexes.clear();

    uint16_t lastIndex = 0;
    float offsetX = 0, offsetY = 0;
    for (auto c : text)
    {
        auto glyphInfo = font->getGlyphInfo(c, offsetX, offsetY);
        offsetX = glyphInfo.offsetX;
        offsetY = glyphInfo.offsetY;

        vertices.emplace_back(glyphInfo.positions[0]);
        vertices.emplace_back(glyphInfo.positions[1]);
        vertices.emplace_back(glyphInfo.positions[2]);
        vertices.emplace_back(glyphInfo.positions[3]);
        uvs.emplace_back(glyphInfo.uvs[0]);
        uvs.emplace_back(glyphInfo.uvs[1]);
        uvs.emplace_back(glyphInfo.uvs[2]);
        uvs.emplace_back(glyphInfo.uvs[3]);
        indexes.push_back(lastIndex);
        indexes.push_back(lastIndex + 1);
        indexes.push_back(lastIndex + 2);
        indexes.push_back(lastIndex);
        indexes.push_back(lastIndex + 2);
        indexes.push_back(lastIndex + 3);

        lastIndex += 4;
    }

    mesh = Mesh::create(node.getScene()->getDevice());

    VertexBufferLayout positionsLayout;
    positionsLayout.addAttribute(3, 0);
    mesh->addDynamicVertexBuffer(positionsLayout, reinterpret_cast<const float *>(vertices.data()), static_cast<uint32_t>(vertices.size()));

    VertexBufferLayout uvsLayout;
    uvsLayout.addAttribute(2, 1);
    mesh->addDynamicVertexBuffer(uvsLayout, reinterpret_cast<const float *>(uvs.data()), static_cast<uint32_t>(uvs.size()));

    mesh->addPart(reinterpret_cast<const void *>(indexes.data()), static_cast<uint32_t>(indexes.size()));

    mesh->setPrimitiveType(PrimitiveType::Triangles);
}

void FontRenderer::updateMesh()
{
    vertices.clear();
    uvs.clear();

    float offsetX = 0, offsetY = 0;
    for (auto c : text)
    {
        auto glyphInfo = font->getGlyphInfo(c, offsetX, offsetY);
        offsetX = glyphInfo.offsetX;
        offsetY = glyphInfo.offsetY;

        vertices.emplace_back(glyphInfo.positions[0]);
        vertices.emplace_back(glyphInfo.positions[1]);
        vertices.emplace_back(glyphInfo.positions[2]);
        vertices.emplace_back(glyphInfo.positions[3]);
        uvs.emplace_back(glyphInfo.uvs[0]);
        uvs.emplace_back(glyphInfo.uvs[1]);
        uvs.emplace_back(glyphInfo.uvs[2]);
        uvs.emplace_back(glyphInfo.uvs[3]);
    }

    mesh->updateDynamicVertexBuffer(0, 0, reinterpret_cast<const float *>(vertices.data()), static_cast<uint32_t>(vertices.size()));
    mesh->updateDynamicVertexBuffer(1, 0, reinterpret_cast<const float *>(uvs.data()), static_cast<uint32_t>(uvs.size()));
}
