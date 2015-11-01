#include "SoloGLSLEffect.h"
#include "SoloGLSLEffectVariable.h"

using namespace solo;


static GLint createProgram(GLuint vs, GLuint fs)
{
	auto program = glCreateProgram();
	glAttachShader(program, vs);
	glAttachShader(program, fs);
	glLinkProgram(program);

	int status;
	glGetProgramiv(program, GL_LINK_STATUS, &status);
	if (status == GL_FALSE)
	{
		int logLength;
		glGetProgramiv(program, GL_INFO_LOG_LENGTH, &logLength);
		std::vector<GLchar> log(logLength);
		glGetProgramInfoLog(program, logLength, nullptr, log.data());
		glDeleteProgram(program);
		SL_THROW(EffectCompilationException, "Failed to link effect program", log.data());
	}

	return program;
}


static GLint compileShader(GLuint type, std::string src)
{
	static std::unordered_map<GLuint, std::string> typeNames =
	{
		{ GL_VERTEX_SHADER, "vertex" },
		{ GL_FRAGMENT_SHADER, "fragment" }
	};

	auto shader = glCreateShader(type);

	auto rawSrc = src.c_str();
	glShaderSource(shader, 1, &rawSrc, nullptr);
	glCompileShader(shader);

	int status;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
	if (status == GL_FALSE)
	{
		int logLength;
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &logLength);
		std::vector<GLchar> log(logLength);
		glGetShaderInfoLog(shader, logLength, nullptr, log.data());
		glDeleteShader(shader);
		SL_THROW(EffectCompilationException, SL_FMT("Failed to compile ", typeNames[type], " shader: "), log.data());
	}

	return shader;
}


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

		auto variable = SL_NEW2(GLSLEffectVariable, name, location, type, index);
		variables[name] = variable;
	}
}