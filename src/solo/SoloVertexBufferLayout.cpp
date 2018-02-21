/*
    Copyright (c) Aleksey Fedotov
    MIT license
*/

#include "SoloVertexBufferLayout.h"

using namespace solo;

void VertexBufferLayout::addAttribute(u32 elementCount, const str &name, VertexAttributeSemantics semantics)
{
    const auto size = static_cast<u32>(sizeof(float) * elementCount);
    const auto offset = attrs.empty() ? 0 : attrs.crbegin()->offset + attrs.crbegin()->size;
    attrs.push_back(VertexAttribute{name, elementCount, size, 0, offset, semantics});
    this->size += size;
}

void VertexBufferLayout::addSemanticAttribute(VertexAttributeSemantics semantics)
{
    switch (semantics)
    {
        case VertexAttributeSemantics::Position:
            addAttribute(3, "sl_Position", VertexAttributeSemantics::Position);
            break;
        case VertexAttributeSemantics::Normal:
            addAttribute(3, "sl_Normal", VertexAttributeSemantics::Normal);
            break;
        case VertexAttributeSemantics::TexCoord:
            addAttribute(2, "sl_TexCoord", VertexAttributeSemantics::TexCoord);
            break;
        case VertexAttributeSemantics::Tangent:
            addAttribute(2, "sl_Tangent", VertexAttributeSemantics::Tangent);
            break;
        case VertexAttributeSemantics::Binormal:
            addAttribute(2, "sl_Binormal", VertexAttributeSemantics::Binormal);
            break;
        default:
            return panic("Unknown vertex attribute semantics");
    }
}
