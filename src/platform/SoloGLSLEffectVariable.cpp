#include "SoloGLSLEffectVariable.h"

using namespace solo;

GLSLEffectVariable::GLSLEffectVariable(const std::string& name, GLint location, GLenum type, unsigned index):
	EffectVariable(name),
	_location(location),
	_type(type),
	_index(index)
{
}