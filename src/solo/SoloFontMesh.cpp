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
    if (newText == text_)
        return;

    const auto oldLength = text_.size();
    const auto newLength = newText.size();
    text_ = newText;

    if (font_ && newLength > 0)
    {
        if (newLength == oldLength && mesh_)
            updateMesh();
        else
            rebuildMesh();
    }
    else
        mesh_ = nullptr;
}

FontMesh::FontMesh(Device *device, sptr<Font> font):
    device_(device),
    font_(font)
{
}

void FontMesh::rebuildMesh()
{
    vertices_.clear();
    uvs_.clear();
    indexes_.clear();

    u16 lastIndex = 0;
    float offsetX = 0, offsetY = 0;
    for (auto c : text_)
    {
        auto glyphInfo = font_->glyphInfo(c, offsetX, offsetY);
        offsetX = glyphInfo.offsetX;
        offsetY = glyphInfo.offsetY;

        vertices_.emplace_back(glyphInfo.positions[0]);
        vertices_.emplace_back(glyphInfo.positions[1]);
        vertices_.emplace_back(glyphInfo.positions[2]);
        vertices_.emplace_back(glyphInfo.positions[3]);
        uvs_.emplace_back(glyphInfo.uvs[0]);
        uvs_.emplace_back(glyphInfo.uvs[1]);
        uvs_.emplace_back(glyphInfo.uvs[2]);
        uvs_.emplace_back(glyphInfo.uvs[3]);
        indexes_.push_back(lastIndex);
        indexes_.push_back(lastIndex + 1);
        indexes_.push_back(lastIndex + 2);
        indexes_.push_back(lastIndex);
        indexes_.push_back(lastIndex + 2);
        indexes_.push_back(lastIndex + 3);

        lastIndex += 4;
    }

    mesh_ = Mesh::create(device_);

    VertexBufferLayout positionsLayout;
    positionsLayout.addAttribute(VertexAttributeSemantics::Position);
    mesh_->addDynamicVertexBuffer(positionsLayout, vertices_.data(), static_cast<u32>(vertices_.size()));

    VertexBufferLayout uvsLayout;
    uvsLayout.addAttribute(VertexAttributeSemantics::TexCoord);
    mesh_->addDynamicVertexBuffer(uvsLayout, uvs_.data(), static_cast<u32>(uvs_.size()));

    mesh_->addPart(reinterpret_cast<const void *>(indexes_.data()), static_cast<u32>(indexes_.size()));
    mesh_->setPrimitiveType(PrimitiveType::Triangles);
}

void FontMesh::updateMesh()
{
    vertices_.clear();
    uvs_.clear();

    float offsetX = 0, offsetY = 0;
    for (auto c : text_)
    {
        auto glyphInfo = font_->glyphInfo(c, offsetX, offsetY);
        offsetX = glyphInfo.offsetX;
        offsetY = glyphInfo.offsetY;

        vertices_.emplace_back(glyphInfo.positions[0]);
        vertices_.emplace_back(glyphInfo.positions[1]);
        vertices_.emplace_back(glyphInfo.positions[2]);
        vertices_.emplace_back(glyphInfo.positions[3]);
        uvs_.emplace_back(glyphInfo.uvs[0]);
        uvs_.emplace_back(glyphInfo.uvs[1]);
        uvs_.emplace_back(glyphInfo.uvs[2]);
        uvs_.emplace_back(glyphInfo.uvs[3]);
    }

    mesh_->updateDynamicVertexBuffer(0, 0, reinterpret_cast<const float *>(vertices_.data()), static_cast<u32>(vertices_.size()));
    mesh_->updateDynamicVertexBuffer(1, 0, reinterpret_cast<const float *>(uvs_.data()), static_cast<u32>(uvs_.size()));
}
