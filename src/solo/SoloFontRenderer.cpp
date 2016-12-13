/*
    Copyright (c) Aleksey Fedotov

    This software is provided 'as-is', without any express or implied
    warranty. In no event will the authors be held liable for any damages
    arising from the use of this software.

    Permission is granted to anyone to use this software for any purpose,
    including commercial applications, and to alter it and redistribute it
    freely, subject to the following restrictions:

    1. The origin of this software must not be misrepresented; you must not
        claim that you wrote the original software. If you use this software
        in a product, an acknowledgment in the product documentation would be
        appreciated but is not required.
    2. Altered source versions must be plainly marked as such, and must not be
        misrepresented as being the original software.
    3. This notice may not be removed or altered from any source distribution.
*/

#include "SoloFontRenderer.h"
#include "SoloMesh.h"
#include "SoloFont.h"
#include "SoloMaterial.h"
#include "SoloRenderQueue.h"
#include "SoloRectTexture.h"

using namespace solo;


FontRenderer::FontRenderer(const Node &node):
    ComponentBase(node)
{
    auto effect = Effect::create(node.getScene()->getDevice(), EffectPrefab::Font);
    material = Material::create(node.getScene()->getDevice(), effect);
    material->setFaceCull(FaceCull::All);
    material->bindWorldViewProjectionMatrixParameter("worldViewProjMatrix");
    material->setTransparent(true);

    renderQueue = KnownRenderQueues::Transparent;
}


void FontRenderer::render(RenderContext &context)
{
    if (!mesh)
        return;
    material->apply(context);
    mesh->draw(material->getEffect());
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

    VertexBufferLayout layout1, layout2;
    layout1.add(VertexBufferLayoutSemantics::Position, 3);
    layout2.add(VertexBufferLayoutSemantics::TexCoord0, 2);

    mesh = Mesh::create(node.getScene()->getDevice());
    mesh->addDynamicVertexBuffer(layout1, reinterpret_cast<const float *>(vertices.data()), static_cast<uint32_t>(vertices.size()));
    mesh->addDynamicVertexBuffer(layout2, reinterpret_cast<const float *>(uvs.data()), static_cast<uint32_t>(uvs.size()));
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
