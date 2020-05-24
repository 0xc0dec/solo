/* 
 * Copyright (c) Aleksey Fedotov 
 * MIT license 
 */

#include "SoloFontMesh.h"
#include "SoloFont.h"
#include "SoloMesh.h"

using namespace solo;

auto FontMesh::fromFont(Device *device, sptr<Font> font) -> sptr<FontMesh>
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

    u32 lastIndex = 0;
    float offsetX = 0, offsetY = 0;
    for (auto c : text_)
    {
        auto glyphInfo = font_->glyphInfo(c, offsetX, offsetY);
        offsetX = glyphInfo.offsetX;
        offsetY = glyphInfo.offsetY;

        vertices_.push_back(glyphInfo.positions[0].x());
    	vertices_.push_back(glyphInfo.positions[0].y());
    	vertices_.push_back(glyphInfo.positions[0].z());
    	vertices_.push_back(glyphInfo.positions[1].x());
    	vertices_.push_back(glyphInfo.positions[1].y());
    	vertices_.push_back(glyphInfo.positions[1].z());
    	vertices_.push_back(glyphInfo.positions[2].x());
    	vertices_.push_back(glyphInfo.positions[2].y());
    	vertices_.push_back(glyphInfo.positions[2].z());
    	vertices_.push_back(glyphInfo.positions[3].x());
    	vertices_.push_back(glyphInfo.positions[3].y());
    	vertices_.push_back(glyphInfo.positions[3].z());
        uvs_.push_back(glyphInfo.uvs[0].x());
    	uvs_.push_back(glyphInfo.uvs[0].y());
    	uvs_.push_back(glyphInfo.uvs[1].x());
    	uvs_.push_back(glyphInfo.uvs[1].y());
    	uvs_.push_back(glyphInfo.uvs[2].x());
    	uvs_.push_back(glyphInfo.uvs[2].y());
    	uvs_.push_back(glyphInfo.uvs[3].x());
    	uvs_.push_back(glyphInfo.uvs[3].y());
        indexes_.push_back(lastIndex);
        indexes_.push_back(lastIndex + 1);
        indexes_.push_back(lastIndex + 2);
        indexes_.push_back(lastIndex);
        indexes_.push_back(lastIndex + 2);
        indexes_.push_back(lastIndex + 3);

        lastIndex += 4;
    }

    mesh_ = Mesh::empty(device_);

    VertexBufferLayout positionsLayout;
    positionsLayout.addAttribute(VertexAttributeUsage::Position);
    mesh_->addDynamicVertexBuffer(positionsLayout, vertices_, static_cast<u32>(vertices_.size()));

    VertexBufferLayout uvsLayout;
    uvsLayout.addAttribute(VertexAttributeUsage::TexCoord);
    mesh_->addDynamicVertexBuffer(uvsLayout, uvs_, static_cast<u32>(uvs_.size()));

    mesh_->addPart(indexes_, static_cast<u32>(indexes_.size()));
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

        vertices_.push_back(glyphInfo.positions[0].x());
    	vertices_.push_back(glyphInfo.positions[0].y());
    	vertices_.push_back(glyphInfo.positions[0].z());
    	vertices_.push_back(glyphInfo.positions[1].x());
    	vertices_.push_back(glyphInfo.positions[1].y());
    	vertices_.push_back(glyphInfo.positions[1].z());
    	vertices_.push_back(glyphInfo.positions[2].x());
    	vertices_.push_back(glyphInfo.positions[2].y());
    	vertices_.push_back(glyphInfo.positions[2].z());
    	vertices_.push_back(glyphInfo.positions[3].x());
    	vertices_.push_back(glyphInfo.positions[3].y());
    	vertices_.push_back(glyphInfo.positions[3].z());
        uvs_.push_back(glyphInfo.uvs[0].x());
    	uvs_.push_back(glyphInfo.uvs[0].y());
    	uvs_.push_back(glyphInfo.uvs[1].x());
    	uvs_.push_back(glyphInfo.uvs[1].y());
    	uvs_.push_back(glyphInfo.uvs[2].x());
    	uvs_.push_back(glyphInfo.uvs[2].y());
    	uvs_.push_back(glyphInfo.uvs[3].x());
    	uvs_.push_back(glyphInfo.uvs[3].y());
    }

    mesh_->updateVertexBuffer(0, 0, reinterpret_cast<const float *>(vertices_.data()), static_cast<u32>(vertices_.size()));
    mesh_->updateVertexBuffer(1, 0, reinterpret_cast<const float *>(uvs_.data()), static_cast<u32>(uvs_.size()));
}
