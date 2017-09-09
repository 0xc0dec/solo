/*
    Copyright (c) Aleksey Fedotov
    MIT license
*/

#include "SoloOpenGLEffect.h"

#ifdef SL_OPENGL_RENDERER

#include "SoloOpenGLPrefabShaders.h"
#include <unordered_map>

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
}

gl::Effect::~Effect()
{
    glDeleteProgram(handle);
}

#endif
