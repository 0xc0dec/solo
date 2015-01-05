#include "SoloGLSLEffect.h"
#include "../SoloBaseInternal.h"
#include "../SoloLog.h"

using namespace solo;


GLSLEffect::GLSLEffect(const std::string &vsSrc, const std::string &fsSrc)
	: Effect(vsSrc, fsSrc)
{
	auto vs = tryCreateShader(GL_VERTEX_SHADER, vsSrc);
	if (vs < 0)
		return;

	auto fs = tryCreateShader(GL_FRAGMENT_SHADER, fsSrc);
	if (fs < 0)
		return;

	auto program = tryCreateProgram(vs, fs);
	if (program < 0)
		return;

	_valid = true;
	_program = program;

	glDetachShader(_program, vs);
	glDetachShader(_program, fs);
	glDeleteShader(vs);
	glDeleteShader(fs);

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


ptr<GLSLEffect> GLSLEffect::create(std::string const& vsSrc, std::string const& fsSrc)
{
	return ALLOC_WITH_DELETER(GLSLEffect, vsSrc, fsSrc);
}


GLint GLSLEffect::tryCreateProgram(GLuint vs, GLuint fs)
{
	auto program = glCreateProgram();
	glAttachShader(program, vs);
	glAttachShader(program, fs);
	glLinkProgram(program);

	int logLength;
	glGetProgramiv(program, GL_INFO_LOG_LENGTH, &logLength);
	auto log = new char[logLength];
	glGetProgramInfoLog(program, logLength, nullptr, log);
	if (logLength > 1)
		appendToLog(log);
	delete[] log;
	
	int status;
	glGetProgramiv(program, GL_COMPILE_STATUS, &status);
	if (status == GL_FALSE)
	{
		glDeleteProgram(program);
		return -1;
	}
	
	return program;
}


GLint GLSLEffect::tryCreateShader(GLuint type, std::string src)
{
	auto shader = glCreateShader(type);

	const GLchar *rawSrc = src.c_str();
	glShaderSource(shader, 1, &rawSrc, nullptr);
	glCompileShader(shader);

	int logLength;
	glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &logLength);
	auto log = new char[logLength];
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
