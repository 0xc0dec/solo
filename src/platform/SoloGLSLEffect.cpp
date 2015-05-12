#include "SoloGLSLEffect.h"
#include "SoloGLSLEffectVariable.h"

using namespace solo;


GLSLEffect::GLSLEffect(const std::string &vsSrc, const std::string &fsSrc)
{
	auto vs = compileShader(GL_VERTEX_SHADER, vsSrc);
	auto fs = compileShader(GL_FRAGMENT_SHADER, fsSrc);
	program = createProgram(vs, fs);
	
	deleteShader(vs);
	deleteShader(fs);

	discoverVariables();
}


GLSLEffect::~GLSLEffect()
{
	if (program)
		glDeleteProgram(program);
}


void GLSLEffect::bind()
{
	if (program)
		glUseProgram(program);
}


GLint GLSLEffect::createProgram(GLuint vs, GLuint fs)
{
	auto program = glCreateProgram();
	glAttachShader(program, vs);
	glAttachShader(program, fs);
	glLinkProgram(program);

	int logLength;
	glGetProgramiv(program, GL_INFO_LOG_LENGTH, &logLength);
	
	std::vector<GLchar> log(logLength);
	glGetProgramInfoLog(program, logLength, nullptr, log.data());
	
	int status;
	glGetProgramiv(program, GL_COMPILE_STATUS, &status);
	if (status == GL_FALSE)
	{
		glDeleteProgram(program);
		THROW(EffectCompilationException, "Failed to link effect program", log.data());
	}
	
	return program;
}


GLint GLSLEffect::compileShader(GLuint type, std::string src)
{
	auto shader = glCreateShader(type);

	auto rawSrc = src.c_str();
	glShaderSource(shader, 1, &rawSrc, nullptr);
	glCompileShader(shader);

	int logLength;
	glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &logLength);
	
	std::vector<GLchar> log(logLength);
	glGetShaderInfoLog(shader, logLength, nullptr, log.data());

	int status;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
	if (status == GL_FALSE)
	{
		glDeleteShader(shader);
		THROW(EffectCompilationException, "Failed to compile shader", log.data());
	}

	return shader;
}


void GLSLEffect::deleteShader(GLuint shader)
{
	glDetachShader(program, shader);
	glDeleteShader(shader);
}


void GLSLEffect::discoverVariables()
{
	GLint activeUniforms, nameMaxLength;
	glGetProgramiv(program, GL_ACTIVE_UNIFORMS, &activeUniforms);
	if (activeUniforms <= 0)
		return;
	glGetProgramiv(program, GL_ACTIVE_UNIFORM_MAX_LENGTH, &nameMaxLength);
	if (nameMaxLength <= 0)
		return;

	std::vector<GLchar> rawName(nameMaxLength + 1);
	GLint size;
	GLenum type;
	unsigned samplerIndex = 0;
	for (size_t i = 0; i < activeUniforms; ++i)
	{
		glGetActiveUniform(program, i, nameMaxLength, nullptr, &size, &type, rawName.data());
		rawName[nameMaxLength] = '\0';
		std::string name = rawName.data();

		// Strip away possible square brackets for array uniforms since the brackets'
		// presence is not consistent across platforms.
		auto bracketIndex = name.find('[');
		if (bracketIndex != std::string::npos)
			name.erase(bracketIndex);

		auto location = glGetUniformLocation(program, rawName.data());
		unsigned index = 0;
		if (type == GL_SAMPLER_2D)
		{
			index = samplerIndex;
			samplerIndex += size;
		}
		// TODO other types of samplers

		auto variable = NEW<GLSLEffectVariable>(name, location, type, index);
		variables[name] = variable;
	}
}