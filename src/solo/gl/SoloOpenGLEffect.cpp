/*
    Copyright (c) Aleksey Fedotov
    MIT license
*/

#include "SoloOpenGLEffect.h"

#ifdef SL_OPENGL_RENDERER

#include "SoloOpenGLPrefabShaders.h"
#include <unordered_map>
#include <vector>

using namespace solo;

auto gl::Effect::createFromPrefab(EffectPrefab prefab) -> sptr<Effect>
{
    switch (prefab)
    {
        case EffectPrefab::Skybox:
            return std::make_shared<Effect>(
                PrefabShaders::Vertex::skybox, std::strlen(PrefabShaders::Vertex::skybox),
                PrefabShaders::Fragment::skybox, std::strlen(PrefabShaders::Fragment::skybox));
        case EffectPrefab::Font:
            return std::make_shared<Effect>(
                PrefabShaders::Vertex::positionAndTexCoord, std::strlen(PrefabShaders::Vertex::positionAndTexCoord),
                PrefabShaders::Fragment::font, std::strlen(PrefabShaders::Fragment::font));
        default:
            SL_PANIC("Unknown effect prefab");
            break;
    }
}

static auto compileShader(GLuint type, const void *src, uint32_t length) -> GLint
{
    static std::unordered_map<GLuint, std::string> typeNames =
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
        std::vector<GLchar> log(logLength);
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
        std::vector<GLchar> log(logLength);
        glGetProgramInfoLog(program, logLength, nullptr, log.data());
        glDeleteProgram(program);
        SL_PANIC(SL_FMT("Failed to link program:\n", log.data()));
    }

    return program;
}

gl::Effect::Effect(const void *vsSrc, uint32_t vsSrcLen, const void *fsSrc, uint32_t fsSrcLen)
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

gl::Effect::~Effect()
{
    glDeleteProgram(handle);
}

auto gl::Effect::getUniformInfo(const std::string &name) -> UniformInfo
{
    if (uniforms.count(name))
        return uniforms.at(name);
    SL_PANIC(SL_FMT("Uniform ", name, " not found"));
    return {};
}

auto gl::Effect::getAttributeInfo(const std::string &name) -> AttributeInfo
{
    if (attributes.count(name))
        return attributes.at(name);
    SL_PANIC(SL_FMT("Attribute ", name, " not found"));
    return {};
}

void gl::Effect::introspectUniforms()
{
    GLint activeUniforms;
    glGetProgramiv(handle, GL_ACTIVE_UNIFORMS, &activeUniforms);
    if (activeUniforms <= 0)
        return;

    GLint nameMaxLength;
    glGetProgramiv(handle, GL_ACTIVE_UNIFORM_MAX_LENGTH, &nameMaxLength);
    if (nameMaxLength <= 0)
        return;

    std::vector<GLchar> nameArr(nameMaxLength + 1);
    uint32_t samplerIndex = 0;
    for (GLint i = 0; i < activeUniforms; ++i)
    {
        GLint size;
        GLenum type;
        glGetActiveUniform(handle, i, nameMaxLength, nullptr, &size, &type, nameArr.data());
        
        nameArr[nameMaxLength] = '\0';
        std::string name = nameArr.data();

        // Strip away possible square brackets for array uniforms,
        // they are sometimes present on some platforms
        const auto bracketIndex = name.find('[');
        if (bracketIndex != std::string::npos)
            name.erase(bracketIndex);

        uniforms[name].location = glGetUniformLocation(handle, nameArr.data());
        uniforms.at(name).samplerIndex = 0;

        uint32_t idx = 0;
        if (type == GL_SAMPLER_2D || type == GL_SAMPLER_CUBE) // TODO other types of samplers
        {
            idx = samplerIndex;
            samplerIndex += size;
            uniforms.at(name).samplerIndex = idx;
        }
    }
}

void gl::Effect::introspectAttributes()
{
    GLint activeAttributes;
    glGetProgramiv(handle, GL_ACTIVE_ATTRIBUTES, &activeAttributes);

    GLint nameMaxLength;
    glGetProgramiv(handle, GL_ACTIVE_UNIFORM_MAX_LENGTH, &nameMaxLength);
    if (nameMaxLength <= 0)
        return;

    std::vector<GLchar> nameArr(nameMaxLength + 1);
    for (GLint i = 0; i < activeAttributes; ++i)
    {
        GLint size;
        GLenum type;
        glGetActiveAttrib(handle, i, nameMaxLength, nullptr, &size, &type, nameArr.data());

        std::string name = nameArr.data();
        attributes[name].location = glGetAttribLocation(handle, name.c_str());
    }
}

#endif
