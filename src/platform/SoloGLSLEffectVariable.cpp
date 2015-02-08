#include <GL/glew.h>
#include "SoloGLSLEffectVariable.h"
#include "SoloMatrix.h"
#include "SoloVector2.h"
#include "SoloVector3.h"
#include "SoloVector4.h"

using namespace solo;


GLSLEffectVariable::GLSLEffectVariable(const std::string& name, GLint location, GLenum type, unsigned index):
	EffectVariable(name),
	location(location),
	type(type),
	index(index)
{
}


ptr<GLSLEffectVariable> GLSLEffectVariable::create(const std::string& name, GLint location, GLenum type, unsigned index)
{
	return NEW2(GLSLEffectVariable, name, location, type, index);
}


void GLSLEffectVariable::setValue(float value)
{
	glUniform1f(location, value);
}


void GLSLEffectVariable::setValue(const float* values, unsigned count)
{
	glUniform1fv(location, count, values);
}


void GLSLEffectVariable::setValue(int value)
{
	glUniform1i(location, value);
}


void GLSLEffectVariable::setValue(const int* values, unsigned count)
{
	glUniform1iv(location, count, values);
}


void GLSLEffectVariable::setValue(const Matrix& value)
{
	glUniformMatrix4fv(location, 1, GL_FALSE, value.m);
}


void GLSLEffectVariable::setValue(const Matrix* values, unsigned count)
{
	glUniformMatrix4fv(location, 1, GL_FALSE, reinterpret_cast<const GLfloat*>(values));
}


void GLSLEffectVariable::setValue(const Vector2& value)
{
	glUniform2f(location, value.x, value.y);
}


void GLSLEffectVariable::setValue(const Vector2* values, unsigned count)
{
	glUniform2fv(location, count, reinterpret_cast<const GLfloat*>(values));
}


void GLSLEffectVariable::setValue(const Vector3& value)
{
	glUniform3f(location, value.x, value.y, value.z);
}


void GLSLEffectVariable::setValue(const Vector3* values, unsigned count)
{
	glUniform3fv(location, count, reinterpret_cast<const GLfloat*>(values));
}


void GLSLEffectVariable::setValue(const Vector4& value)
{
	glUniform4f(location, value.x, value.y, value.z, value.z);
}


void GLSLEffectVariable::setValue(const Vector4* values, unsigned count)
{
	glUniform4fv(location, count, reinterpret_cast<const GLfloat*>(values));
}