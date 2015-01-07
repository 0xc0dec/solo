#include "SoloGLSLEffect.h"
#include "SoloGLSLEffectVariable.h"
#include "SoloException.h"
#include "../SoloLog.h"

using namespace solo;


GLSLEffect::GLSLEffect(const std::string &vsSrc, const std::string &fsSrc):
	Effect(vsSrc, fsSrc)
{
	auto vs = createShader(GL_VERTEX_SHADER, vsSrc);
	if (vs < 0)
		return;

	auto fs = createShader(GL_FRAGMENT_SHADER, fsSrc);
	if (fs < 0)
		return;

	if (!createProgram(vs, fs))
		return;

	_valid = true;

	deleteShader(vs);
	deleteShader(fs);

	discoverVariables();

	DEBUG("Created effect ", _id);
}


GLSLEffect::~GLSLEffect()
{
	if (_valid)
	{
		glDeleteProgram(_program);
		DEBUG("Destroyed effect ", _id);
	}
}


void GLSLEffect::bind()
{
	glUseProgram(_program);
}


ptr<EffectVariable> GLSLEffect::findVariable(const std::string& name)
{
	auto where = _variables.find(name);
	if (where != _variables.end())
		return where->second;
	return nullptr;
}


bool GLSLEffect::createProgram(GLuint vs, GLuint fs)
{
	_program = glCreateProgram();
	glAttachShader(_program, vs);
	glAttachShader(_program, fs);
	glLinkProgram(_program);

	int logLength;
	glGetProgramiv(_program, GL_INFO_LOG_LENGTH, &logLength);
	auto log = new char[logLength];
	glGetProgramInfoLog(_program, logLength, nullptr, log);
	if (logLength > 1)
		appendToLog(log);
	delete[] log;
	
	int status;
	glGetProgramiv(_program, GL_COMPILE_STATUS, &status);
	if (status == GL_FALSE)
	{
		glDeleteProgram(_program);
		return false;
	}
	
	return true;
}


GLint GLSLEffect::createShader(GLuint type, std::string src)
{
	auto shader = glCreateShader(type);

	const GLchar *rawSrc = src.c_str();
	glShaderSource(shader, 1, &rawSrc, nullptr);
	glCompileShader(shader);

	int logLength;
	glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &logLength);
	auto log = new GLchar[logLength];
	glGetShaderInfoLog(shader, logLength, nullptr, log);
	if (logLength > 1)
		appendToLog(log);
	delete[] log;

	int status;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
	if (status == GL_FALSE)
	{
		glDeleteShader(shader);
		return -1;
	}

	return shader;
}


void GLSLEffect::deleteShader(GLuint shader)
{
	glDetachShader(_program, shader);
	glDeleteShader(shader);
}


void GLSLEffect::discoverVariables()
{
	GLint activeUniforms, nameMaxLength;
	glGetProgramiv(_program, GL_ACTIVE_UNIFORMS, &activeUniforms);
	if (activeUniforms <= 0)
		return;
	glGetProgramiv(_program, GL_ACTIVE_UNIFORM_MAX_LENGTH, &nameMaxLength);
	if (nameMaxLength <= 0)
		return;

	auto rawName = new GLchar[nameMaxLength + 1];
	GLint size;
	GLenum type;
	unsigned samplerIndex = 0;
	for (int i = 0; i < activeUniforms; ++i)
	{
		glGetActiveUniform(_program, i, nameMaxLength, nullptr, &size, &type, rawName);
		rawName[nameMaxLength] = '\0';
		std::string name = rawName;

		// Strip away possible square brackets for array uniforms since the brackets'
		// presence is not consistent across platforms.
		auto bracketIndex = name.find('[');
		if (bracketIndex != std::string::npos)
			name.erase(bracketIndex);

		auto location = glGetUniformLocation(_program, rawName);
		unsigned index = 0;
		if (type == GL_SAMPLER_2D)
		{
			index = samplerIndex;
			samplerIndex += size;
		}

		auto variable = NEW<GLSLEffectVariable>(name, location, type, index);
		_variables[name] = variable;
	}

	delete[] rawName;
}