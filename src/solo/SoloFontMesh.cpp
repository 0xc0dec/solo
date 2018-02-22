/*
    Copyright (c) Aleksey Fedotov
    MIT license
*/

#include "SoloFontMesh.h"
#include "SoloFont.h"
#include "SoloMesh.h"

using namespace solo;

auto FontMesh::create(Device *device, sptr<Font> font) -> sptr<FontMesh>
{
    return sptr<FontMesh>(new FontMesh(device, font));
}

void FontMesh::setText(const str &newText)
{
    if (newText == text)
        return;

    const auto oldLength = text.size();
    const auto newLength = newText.size();
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

FontMesh::FontMesh(Device *device, sptr<Font> font):
    device(device),
    font(font)
{
}

void FontMesh::rebuildMesh()
{
    vertices.clear();
    uvs.clear();
    indexes.clear();

    u16 lastIndex = 0;
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

    mesh = Mesh::create(device);

    VertexBufferLayout positionsLayout;
    positionsLayout.addSemanticAttribute(VertexAttributeSemantics::Position);
    mesh->addDynamicVertexBuffer(positionsLayout, vertices.data(), static_cast<u32>(vertices.size()));

    VertexBufferLayout uvsLayout;
    uvsLayout.addSemanticAttribute(VertexAttributeSemantics::TexCoord);
    mesh->addDynamicVertexBuffer(uvsLayout, uvs.data(), static_cast<u32>(uvs.size()));

    mesh->addPart(reinterpret_cast<const void *>(indexes.data()), static_cast<u32>(indexes.size()));
    mesh->setPrimitiveType(PrimitiveType::Triangles);
}

void FontMesh::updateMesh()
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

    mesh->updateDynamicVertexBuffer(0, 0, reinterpret_cast<const float *>(vertices.data()), static_cast<u32>(vertices.size()));
    mesh->updateDynamicVertexBuffer(1, 0, reinterpret_cast<const float *>(uvs.data()), static_cast<u32>(uvs.size()));
}
