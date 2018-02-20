/*
    Copyright (c) Aleksey Fedotov
    MIT license
*/

#include "SoloVertexBufferLayout.h"

using namespace solo;

void VertexBufferLayout::addNamedAttribute(u32 elementCount, const str &name)
{
    const auto size = static_cast<u32>(sizeof(float) * elementCount);
    const auto offset = attrs.empty() ? 0 : attrs.crbegin()->offset + attrs.crbegin()->size;
    attrs.push_back(VertexAttribute{name, elementCount, size, 0, offset});
    this->size += size;
}

void VertexBufferLayout::addSemanticAttribute(VertexAttributeSemantics semantics)
{
	switch (semantics)
	{
		case VertexAttributeSemantics::Position:
			addNamedAttribute(3, "sl_Position");
			break;
		case VertexAttributeSemantics::Normal:
			addNamedAttribute(3, "sl_Normal");
			break;
		case VertexAttributeSemantics::TexCoord:
			addNamedAttribute(2, "sl_TexCoord");
			break;
		default:
			return panic("Unknown vertex attribute semantics");
	}
}
