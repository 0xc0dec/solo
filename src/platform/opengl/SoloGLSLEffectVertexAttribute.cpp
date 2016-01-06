#include "SoloGLSLEffectVertexAttribute.h"

using namespace solo;


GLSLEffectVertexAttribute::GLSLEffectVertexAttribute(const std::string& name, GLint location):
	EffectVertexAttribute(name),
	location(location)
{
}
