/*
    Copyright (c) Aleksey Fedotov
    MIT license
*/

#include "SoloOpenGLEffect.h"

#ifdef SL_OPENGL_RENDERER

using namespace solo;

static auto compileShader(GLuint type, const void *src, u32 length) -> GLint
{
    static umap<GLuint, str> typeNames =
    {
        {GL_VERTEX_SHADER, "vertex"},
        {GL_FRAGMENT_SHADER, "fragment"}
    };

    const auto shader = glCreateShader(type);

    GLint len = length;
    glShaderSource(shader, 1, reinterpret_cast<const GLchar* const*>(&src), &len);
    glCompileShader(shader);

    GLint status;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
    if (status == GL_FALSE)
    {
        GLint logLength;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &logLength);
        vec<GLchar> log(logLength);
        glGetShaderInfoLog(shader, logLength, nullptr, log.data());
        glDeleteShader(shader);
        SL_PANIC(SL_FMT("Failed to compile ", typeNames[type], " shader:\n", log.data()));
    }

    return shader;
}

static auto linkProgram(GLuint vs, GLuint fs) -> GLint
{
    const auto program = glCreateProgram();
    glAttachShader(program, vs);
    glAttachShader(program, fs);
    glLinkProgram(program);

    GLint status;
    glGetProgramiv(program, GL_LINK_STATUS, &status);
    if (status == GL_FALSE)
    {
        GLint logLength;
        glGetProgramiv(program, GL_INFO_LOG_LENGTH, &logLength);
        vec<GLchar> log(logLength);
        glGetProgramInfoLog(program, logLength, nullptr, log.data());
        glDeleteProgram(program);
        SL_PANIC(SL_FMT("Failed to link program:\n", log.data()));
    }

    return program;
}

OpenGLEffect::OpenGLEffect(const void *vsSrc, u32 vsSrcLen, const void *fsSrc, u32 fsSrcLen)
{
    const auto vs = compileShader(GL_VERTEX_SHADER, vsSrc, vsSrcLen);
    const auto fs = compileShader(GL_FRAGMENT_SHADER, fsSrc, fsSrcLen);
    handle = linkProgram(vs, fs);

    glDetachShader(handle, vs);
    glDeleteShader(vs);
    glDetachShader(handle, fs);
    glDeleteShader(fs);

    introspectUniforms();
    introspectAttributes();
}

OpenGLEffect::~OpenGLEffect()
{
    glDeleteProgram(handle);
}

auto OpenGLEffect::getUniformInfo(const str &name) -> UniformInfo
{
    if (uniforms.count(name))
        return uniforms.at(name);
    SL_PANIC(SL_FMT("Uniform ", name, " not found"));
    return {};
}

auto OpenGLEffect::getAttributeInfo(const str &name) -> AttributeInfo
{
    if (attributes.count(name))
        return attributes.at(name);
    SL_PANIC(SL_FMT("Attribute ", name, " not found"));
    return {};
}

void OpenGLEffect::introspectUniforms()
{
    GLint activeUniforms;
    glGetProgramiv(handle, GL_ACTIVE_UNIFORMS, &activeUniforms);
    if (activeUniforms <= 0)
        return;

    GLint nameMaxLength;
    glGetProgramiv(handle, GL_ACTIVE_UNIFORM_MAX_LENGTH, &nameMaxLength);
    if (nameMaxLength <= 0)
        return;

    vec<GLchar> nameArr(nameMaxLength + 1);
    u32 samplerIndex = 0;
    for (GLint i = 0; i < activeUniforms; ++i)
    {
        GLint size;
        GLenum type;
        glGetActiveUniform(handle, i, nameMaxLength, nullptr, &size, &type, nameArr.data());
        
        nameArr[nameMaxLength] = '\0';
        str name = nameArr.data();

        // Strip away possible square brackets for array uniforms,
        // they are sometimes present on some platforms
        const auto bracketIndex = name.find('[');
        if (bracketIndex != str::npos)
            name.erase(bracketIndex);

        uniforms[name].location = glGetUniformLocation(handle, nameArr.data());
        uniforms.at(name).samplerIndex = 0;

        u32 idx = 0;
        if (type == GL_SAMPLER_2D || type == GL_SAMPLER_CUBE) // TODO other types of samplers
        {
            idx = samplerIndex;
            samplerIndex += size;
            uniforms.at(name).samplerIndex = idx;
        }
    }
}

void OpenGLEffect::introspectAttributes()
{
    GLint activeAttributes;
    glGetProgramiv(handle, GL_ACTIVE_ATTRIBUTES, &activeAttributes);

    GLint nameMaxLength;
    glGetProgramiv(handle, GL_ACTIVE_ATTRIBUTE_MAX_LENGTH, &nameMaxLength);
    if (nameMaxLength <= 0)
        return;

    vec<GLchar> nameArr(nameMaxLength + 1);
    for (GLint i = 0; i < activeAttributes; ++i)
    {
        GLint size;
        GLenum type;
        glGetActiveAttrib(handle, i, nameMaxLength, nullptr, &size, &type, nameArr.data());

        str name = nameArr.data();
        attributes[name].location = glGetAttribLocation(handle, name.c_str());
    }
}

#endif
