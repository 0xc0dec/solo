#include "SoloGLSLEffect.h"
#include "SoloGLSLEffectVariable.h"

using namespace solo;


GLSLEffect::GLSLEffect(const std::string &vsSrc, const std::string &fsSrc)
{
	auto vs = createShader(GL_VERTEX_SHADER, vsSrc);
	if (vs < 0)
		return;

	auto fs = createShader(GL_FRAGMENT_SHADER, fsSrc);
	if (fs < 0)
		return;

	if (!createProgram(vs, fs))
		return;

	valid = true;

	deleteShader(vs);
	deleteShader(fs);

	discoverVariables();
}


GLSLEffect::~GLSLEffect()
{
	if (valid)
		glDeleteProgram(program);
}


void GLSLEffect::bind()
{
	glUseProgram(program);
}


EffectVariable* GLSLEffect::findVariable(const std::string& name)
{
	auto where = variables.find(name);
	if (where != variables.end())
		return where->second.get();
	return nullptr;
}


bool GLSLEffect::createProgram(GLuint vs, GLuint fs)
{
	program = glCreateProgram();
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
		return false;
	}
	
	return true;
}


GLint GLSLEffect::createShader(GLuint type, std::string src)
{
	auto shader = glCreateShader(type);

	auto rawSrc = src.c_str();
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

	auto rawName = new GLchar[nameMaxLength + 1];
	GLint size;
	GLenum type;
	unsigned samplerIndex = 0;
	for (int i = 0; i < activeUniforms; ++i)
	{
		glGetActiveUniform(program, i, nameMaxLength, nullptr, &size, &type, rawName);
		rawName[nameMaxLength] = '\0';
		std::string name = rawName;

		// Strip away possible square brackets for array uniforms since the brackets'
		// presence is not consistent across platforms.
		auto bracketIndex = name.find('[');
		if (bracketIndex != std::string::npos)
			name.erase(bracketIndex);

		auto location = glGetUniformLocation(program, rawName);
		unsigned index = 0;
		if (type == GL_SAMPLER_2D)
		{
			index = samplerIndex;
			samplerIndex += size;
		}
		// TODO other types of samplers

		auto variable = GLSLEffectVariableFactory::create(name, location, type, index);
		variables[name] = variable;
	}

	delete[] rawName;
}