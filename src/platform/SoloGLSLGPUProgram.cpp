#include "SoloGLSLGPUProgram.h"
#include "../SoloLog.h"

using namespace solo;


GLSLGPUProgram::GLSLGPUProgram(const str &vsShaderSrc, const str &fsShaderSrc)
	: GPUProgram(vsShaderSrc, fsShaderSrc)
{
	auto vs = _tryCreateShader(GL_VERTEX_SHADER, vsShaderSrc);
	if (vs < 0)
		return;

	auto fs = _tryCreateShader(GL_FRAGMENT_SHADER, fsShaderSrc);
	if (fs < 0)
		return;

	auto program = _tryCreateProgram(vs, fs);
	if (program < 0)
		return;

	_valid = true;
	_program = program;

	glDeleteShader(vs);
	glDeleteShader(fs);

	DEBUG("Successfully compiled gpu program " << _id);
}


GLSLGPUProgram::~GLSLGPUProgram()
{
	if (_valid)
	{
		glDeleteProgram(_program);
		DEBUG("Destroyed gpu program " << _id);
	}
}


GLint GLSLGPUProgram::_tryCreateProgram(GLuint vs, GLuint fs)
{
	auto program = glCreateProgram();
	glAttachShader(program, vs);
	glAttachShader(program, fs);
	glLinkProgram(program);

	s32 logLength;
	glGetProgramiv(program, GL_INFO_LOG_LENGTH, &logLength);
	auto log = new c8[logLength];
	glGetProgramInfoLog(program, logLength, nullptr, log);
	if (logLength > 1)
		_appendToLog(log);
	delete[] log;
	
	s32 status;
	glGetProgramiv(program, GL_COMPILE_STATUS, &status);
	if (status == GL_FALSE)
	{
		glDeleteProgram(program);
		return -1;
	}
	
	return program;
}


GLint GLSLGPUProgram::_tryCreateShader(GLuint type, str src)
{
	auto shader = glCreateShader(type);

	const c8 *rawSrc = src.c_str();
	glShaderSource(shader, 1, &rawSrc, nullptr);
	glCompileShader(shader);

	s32 logLength;
	glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &logLength);
	auto log = new c8[logLength];
	glGetShaderInfoLog(shader, logLength, nullptr, log);
	if (logLength > 1)
		_appendToLog(log);
	delete[] log;

	s32 status;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
	if (status == GL_FALSE)
	{
		glDeleteShader(shader);
		return -1;
	}

	return shader;
}
