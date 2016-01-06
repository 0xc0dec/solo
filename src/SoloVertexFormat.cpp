#include "SoloVertexFormat.h"

using namespace solo;


VertexFormat::VertexFormat(const std::vector<VertexFormatElement>& elements):
	elements(elements)
{
	for (const auto& e: elements)
		vertexSize += e.size * sizeof(float);
}
