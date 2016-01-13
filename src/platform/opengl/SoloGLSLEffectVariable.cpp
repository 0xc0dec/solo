#include "SoloGLSLEffectVariable.h"
#include "SoloMatrix.h"
#include "SoloVector2.h"
#include "SoloVector3.h"
#include "SoloVector4.h"
#include "SoloTexture.h"
#include <GL/glew.h>


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


void GLSLEffectVariable::setFloatArray(const std::vector<float>& value)
{
    glUniform1fv(location, static_cast<GLsizei>(value.size()), value.data());
}


void GLSLEffectVariable::setInt(int value)
{
    glUniform1i(location, value);
}


void GLSLEffectVariable::setIntArray(const std::vector<int>& value)
{
    glUniform1iv(location, static_cast<GLsizei>(value.size()), value.data());
}


void GLSLEffectVariable::setMatrix(const Matrix& value)
{
    glUniformMatrix4fv(location, 1, GL_FALSE, value.m);
}


void GLSLEffectVariable::setMatrixArray(const std::vector<Matrix>& value)
{
    glUniformMatrix4fv(location, static_cast<GLsizei>(value.size()), GL_FALSE, reinterpret_cast<const GLfloat*>(value.data()));  // TODO test this
}


void GLSLEffectVariable::setVector2(const Vector2& value)
{
    glUniform2f(location, value.x, value.y);
}


void GLSLEffectVariable::setVector2Array(const std::vector<Vector2>& value)
{
    glUniform2fv(location, static_cast<GLsizei>(value.size()), reinterpret_cast<const GLfloat*>(value.data()));
}


void GLSLEffectVariable::setVector3(const Vector3& value)
{
    glUniform3f(location, value.x, value.y, value.z);
}


void GLSLEffectVariable::setVector3Array(const std::vector<Vector3>& value)
{
    glUniform3fv(location, static_cast<GLsizei>(value.size()), reinterpret_cast<const GLfloat*>(value.data()));
}


void GLSLEffectVariable::setVector4(const Vector4& value)
{
    glUniform4f(location, value.x, value.y, value.z, value.z);
}


void GLSLEffectVariable::setVector4Array(const std::vector<Vector4>& value)
{
    glUniform4fv(location, static_cast<GLsizei>(value.size()), reinterpret_cast<const GLfloat*>(value.data()));
}


void GLSLEffectVariable::setTexture(const shared<Texture> texture)
{
    glActiveTexture(GL_TEXTURE0 + index);
    glUniform1i(location, index);
    texture->apply();
}


void GLSLEffectVariable::setTextureArray(const std::vector<shared<Texture>>& textures)
{
    // TODO
}