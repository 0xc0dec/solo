#include "SoloGLSLEffect.h"
#include "SoloGLSLEffectVariable.h"
#include "SoloGLSLEffectVertexAttribute.h"

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
        SL_THROW(EffectCompilationException, SL_FMT("Failed to compile ", typeNames[type], " shader"), log.data());
    }

    return shader;
}


GLSLEffect::GLSLEffect(const std::string& vsSrc, const std::string& fsSrc)
{
    auto vs = compileShader(GL_VERTEX_SHADER, vsSrc);
    auto fs = compileShader(GL_FRAGMENT_SHADER, fsSrc);
    program = createProgram(vs, fs);

    deleteShader(vs);
    deleteShader(fs);

    discoverVariables();
    discoverVertexAttributes();
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


void GLSLEffect::unbind()
{
    if (program) // really, why unbinding if we couldn't have been bound anyway
        glUseProgram(0);
}


void GLSLEffect::deleteShader(GLuint shader)
{
    glDetachShader(program, shader);
    glDeleteShader(shader);
}


void GLSLEffect::discoverVariables()
{
    GLint activeUniforms;
    glGetProgramiv(program, GL_ACTIVE_UNIFORMS, &activeUniforms);
    if (activeUniforms <= 0)
        return;

    GLint nameMaxLength;
    glGetProgramiv(program, GL_ACTIVE_UNIFORM_MAX_LENGTH, &nameMaxLength);
    if (nameMaxLength <= 0)
        return;

    std::vector<GLchar> rawName(nameMaxLength + 1);
    GLint size;
    GLenum type;
    unsigned samplerIndex = 0;
    for (GLint i = 0; i < activeUniforms; ++i)
    {
        glGetActiveUniform(program, i, nameMaxLength, nullptr, &size, &type, rawName.data());
        rawName[nameMaxLength] = '\0';
        std::string name = rawName.data();

        // Strip away possible square brackets for array uniforms, they are sometimes present on
        // some platforms
        auto bracketIndex = name.find('[');
        if (bracketIndex != std::string::npos)
            name.erase(bracketIndex);

        auto location = glGetUniformLocation(program, rawName.data());
        unsigned index = 0;
        if (type == GL_SAMPLER_2D || type == GL_SAMPLER_CUBE) // TODO other types of samplers
        {
            index = samplerIndex;
            samplerIndex += size;
        }

        auto variable = SL_NEW_SHARED(GLSLEffectVariable, name, location, type, index);
        variables[name] = variable;
    }
}


void GLSLEffect::discoverVertexAttributes()
{
    GLint activeAttributes = 0;
    glGetProgramiv(program, GL_ACTIVE_UNIFORMS, &activeAttributes);
    if (activeAttributes <= 0)
        return;

    GLint nameMaxLength = 0;
    glGetProgramiv(program, GL_ACTIVE_ATTRIBUTE_MAX_LENGTH, &nameMaxLength);
    if (nameMaxLength <= 0)
        return;

    GLint attribSize;
    GLenum attribType;
    std::vector<GLchar> rawName(nameMaxLength + 1);
    for (GLint i = 0; i < activeAttributes; ++i)
    {
        glGetActiveAttrib(program, i, nameMaxLength, nullptr, &attribSize, &attribType, rawName.data());
        rawName[nameMaxLength] = '\0';
        auto location = glGetAttribLocation(program, rawName.data());
        std::string name = rawName.data();
        auto attrib = SL_NEW_SHARED(GLSLEffectVertexAttribute, name, location);
        vertexAttributes[name] = attrib;
    }
}
