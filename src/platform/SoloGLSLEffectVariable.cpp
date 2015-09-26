#include <GL/glew.h>
#include "SoloGLSLEffectVariable.h"
#include "SoloMatrix.h"
#include "SoloVector2.h"
#include "SoloVector3.h"
#include "SoloVector4.h"
#include "SoloTexture.h"

using namespace solo;


GLSLEffectVariable::GLSLEffectVariable(const std::string& name, GLint location, GLenum type, unsigned index):
	EffectVariable(name),
	location(location),
	type(type),
	index(index)
{
}


void GLSLEffectVariable::setFloat(float value)
{
	glUniform1f(location, value);
}


void GLSLEffectVariable::setFloatArray(const float* values, unsigned count)
{
	glUniform1fv(location, count, values);
}


void GLSLEffectVariable::setInt(int value)
{
	glUniform1i(location, value);
}


void GLSLEffectVariable::setIntArray(const int* values, unsigned count)
{
	glUniform1iv(location, count, values);
}


void GLSLEffectVariable::setMatrix(const Matrix& value)
{
	glUniformMatrix4fv(location, 1, GL_FALSE, value.m);
}


void GLSLEffectVariable::setMatrixArray(const Matrix* values, unsigned count)
{
	glUniformMatrix4fv(location, count, GL_FALSE, reinterpret_cast<const GLfloat*>(values));
}


void GLSLEffectVariable::setVector2(const Vector2& value)
{
	glUniform2f(location, value.x, value.y);
}


void GLSLEffectVariable::setVector2Array(const Vector2* values, unsigned count)
{
	glUniform2fv(location, count, reinterpret_cast<const GLfloat*>(values));
}


void GLSLEffectVariable::setVector3(const Vector3& value)
{
	glUniform3f(location, value.x, value.y, value.z);
}


void GLSLEffectVariable::setVector3Array(const Vector3* values, unsigned count)
{
	glUniform3fv(location, count, reinterpret_cast<const GLfloat*>(values));
}


void GLSLEffectVariable::setVector4(const Vector4& value)
{
	glUniform4f(location, value.x, value.y, value.z, value.z);
}


void GLSLEffectVariable::setVector4Array(const Vector4* values, unsigned count)
{
	glUniform4fv(location, count, reinterpret_cast<const GLfloat*>(values));
}


void GLSLEffectVariable::setTexture(const shared<Texture> texture)
{
	glActiveTexture(GL_TEXTURE0 + index);
	glUniform1i(location, index);
	texture->apply();
}


void GLSLEffectVariable::setTextureArray(const std::vector<shared<Texture>>& textures, unsigned count)
{
	// TODO
}