/* 
 * Copyright (c) Aleksey Fedotov 
 * MIT license 
 */

#include "SoloVertexBufferLayout.h"

using namespace solo;

void VertexBufferLayout::addAttribute(u32 elementCount, const str &name, VertexAttributeUsage usage)
{
    const auto size = static_cast<u32>(sizeof(float) * elementCount);
    const auto offset = attrs_.empty() ? 0 : attrs_.crbegin()->offset + attrs_.crbegin()->size;
    attrs_.push_back(VertexAttribute{name, elementCount, size, 0, offset, usage});
    this->elementCount_ += elementCount;
    this->size_ += size;
}

void VertexBufferLayout::addAttribute(VertexAttributeUsage usage)
{
    switch (usage)
    {
        case VertexAttributeUsage::Position:
            addAttribute(3, "sl_Position", VertexAttributeUsage::Position);
            break;
        case VertexAttributeUsage::Normal:
            addAttribute(3, "sl_Normal", VertexAttributeUsage::Normal);
            break;
        case VertexAttributeUsage::TexCoord:
            addAttribute(2, "sl_TexCoord", VertexAttributeUsage::TexCoord);
            break;
        case VertexAttributeUsage::Tangent:
            addAttribute(3, "sl_Tangent", VertexAttributeUsage::Tangent);
            break;
        case VertexAttributeUsage::Binormal:
            addAttribute(3, "sl_Binormal", VertexAttributeUsage::Binormal);
            break;
        default:
            SL_DEBUG_PANIC(true, "Unsupported vertex attribute usage");
    }
}
