/*
    Copyright (c) Aleksey Fedotov
    MIT license
*/

#include "SoloOpenGL.h"

#ifdef SL_OPENGL_RENDERER

#include "SoloRenderer.h"
#include "SoloVector4.h"
#include "SoloMesh.h"
#include "SoloMaterial.h"
#include "SoloTexture.h"

using namespace solo;

auto gl::toPrimitiveType(PrimitiveType type) -> GLenum
{
    switch (type)
    {
        case PrimitiveType::Triangles:
            return GL_TRIANGLES;
        case PrimitiveType::TriangleStrip:
            return GL_TRIANGLE_STRIP;
        case PrimitiveType::Lines:
            return GL_LINES;
        case PrimitiveType::LineStrip:
            return GL_LINE_STRIP;
        case PrimitiveType::Points:
            return GL_POINTS;
        default:
            SL_PANIC("Unknown primitive type");
            return GL_TRIANGLES;
    }
}

auto gl::toCubeTextureFace(CubeTextureFace face) -> GLenum
{
    switch (face)
    {
        case CubeTextureFace::Front:
            return GL_TEXTURE_CUBE_MAP_POSITIVE_Z;
        case CubeTextureFace::Back:
            return GL_TEXTURE_CUBE_MAP_NEGATIVE_Z;
        case CubeTextureFace::Right:
            return GL_TEXTURE_CUBE_MAP_NEGATIVE_X;
        case CubeTextureFace::Left:
            return GL_TEXTURE_CUBE_MAP_POSITIVE_X;
        case CubeTextureFace::Top:
            return GL_TEXTURE_CUBE_MAP_POSITIVE_Y;
        case CubeTextureFace::Bottom:
            return GL_TEXTURE_CUBE_MAP_NEGATIVE_Y;
        default:
            SL_PANIC("Unknown cube texture face");
            return GL_TEXTURE_CUBE_MAP_POSITIVE_Z;
    }
}

auto gl::toInternalTextureFormat(TextureFormat format) -> GLenum
{
    switch (format)
    {
        case TextureFormat::Red:
        case TextureFormat::RGB:
            return GL_RGB;
        case TextureFormat::RGBA:
            return GL_RGBA;
        case TextureFormat::Alpha:
            return GL_ALPHA;
        default:
            SL_PANIC("Unknown texture format");
            return GL_RGB;
    }
}

auto gl::toTextureFormat(TextureFormat format) -> GLenum
{
    switch (format)
    {
        case TextureFormat::Red:
            return GL_RED;
        case TextureFormat::RGB:
            return GL_RGB;
        case TextureFormat::RGBA:
            return GL_RGBA;
        case TextureFormat::Alpha:
            return GL_ALPHA;
        default:
            SL_PANIC("Unknown texture format");
            return GL_RED;
    }
}

void gl::applyMinFilter(GLenum target, uint32_t flags)
{
    GLenum minFilter = 0;
    if (flags & TextureFlags::MinFilterLinear)
        minFilter = GL_LINEAR;
    else if (flags & TextureFlags::MinFilterLinearMipmapLinear)
        minFilter = GL_LINEAR_MIPMAP_LINEAR;
    else if (flags & TextureFlags::MinFilterLinearMipmapNearest)
        minFilter = GL_LINEAR_MIPMAP_NEAREST;
    else if (flags & TextureFlags::MinFilterNearest)
        minFilter = GL_NEAREST;
    else if (flags & TextureFlags::MinFilterNearestMipmapLinear)
        minFilter = GL_NEAREST_MIPMAP_LINEAR;
    else if (flags & TextureFlags::MinFilterNearestMipmapNearest)
        minFilter = GL_NEAREST_MIPMAP_NEAREST;

    if (minFilter)
        glTexParameteri(target, GL_TEXTURE_MIN_FILTER, minFilter);
}

void gl::applyMagFilter(GLenum target, uint32_t flags)
{
    GLenum magFilter = 0;
    if (flags & TextureFlags::MagFilterLinear)
        magFilter = GL_LINEAR;
    else if (flags & TextureFlags::MagFilterLinearMipmapLinear)
        magFilter = GL_LINEAR_MIPMAP_LINEAR;
    else if (flags & TextureFlags::MagFilterLinearMipmapNearest)
        magFilter = GL_LINEAR_MIPMAP_NEAREST;
    else if (flags & TextureFlags::MagFilterNearest)
        magFilter = GL_NEAREST;
    else if (flags & TextureFlags::MagFilterNearestMipmapLinear)
        magFilter = GL_NEAREST_MIPMAP_LINEAR;
    else if (flags & TextureFlags::MagFilterNearestMipmapNearest)
        magFilter = GL_NEAREST_MIPMAP_NEAREST;

    if (magFilter)
        glTexParameteri(target, GL_TEXTURE_MAG_FILTER, magFilter);
}

void gl::applyRectWrap(GLenum target, uint32_t flags)
{
    GLenum wrapS = 0;
    if (flags & TextureFlags::HorizontalWrapClamp)
        wrapS = GL_CLAMP_TO_EDGE;
    else if (flags & TextureFlags::HorizontalWrapRepeat)
        wrapS = GL_REPEAT;
    if (wrapS)
        glTexParameteri(target, GL_TEXTURE_WRAP_S, wrapS);

    GLenum wrapT = 0;
    if (flags & TextureFlags::VerticalWrapClamp)
        wrapT = GL_CLAMP_TO_EDGE;
    else if (flags & TextureFlags::VerticalWrapRepeat)
        wrapT = GL_REPEAT;
    if (wrapT)
        glTexParameteri(target, GL_TEXTURE_WRAP_T, wrapT);
}

void gl::applyDepthWrap(uint32_t flags)
{
    GLenum wrapR = 0;
    if (flags & TextureFlags::DepthWrapClamp)
        wrapR = GL_CLAMP_TO_EDGE;
    else if (flags & TextureFlags::DepthWrapRepeat)
        wrapR = GL_REPEAT;
    if (wrapR)
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, wrapR);
}

void gl::clear(bool color, bool depth, const Vector4 &clearColor)
{
    if (color)
        glClearColor(clearColor.x, clearColor.y, clearColor.z, clearColor.w);
    GLbitfield flags = (color ? GL_COLOR_BUFFER_BIT : 0) | (depth ? GL_DEPTH_BUFFER_BIT : 0);
    glClear(flags);
}

void gl::setViewport(const Vector4 &viewport)
{
    glViewport(viewport.x, viewport.y, viewport.z, viewport.w);
}

void gl::setDepthWrite(bool enabled)
{
    glDepthMask(enabled ? GL_TRUE : GL_FALSE);
}

void gl::setDepthTest(bool enabled)
{
    enabled ? glEnable(GL_DEPTH_TEST) : glDisable(GL_DEPTH_TEST);
}

void gl::setDepthFunction(DepthFunction func)
{
    GLenum glfunc = 0;
    switch (func)
    {
        case DepthFunction::Never:
            glfunc = GL_NEVER;
            break;
        case DepthFunction::Less:
            glfunc = GL_LESS;
            break;
        case DepthFunction::Equal:
            glfunc = GL_EQUAL;
            break;
        case DepthFunction::LEqual:
            glfunc = GL_LEQUAL;
            break;
        case DepthFunction::Greater:
            glfunc = GL_GREATER;
            break;
        case DepthFunction::NotEqual:
            glfunc = GL_NOTEQUAL;
            break;
        case DepthFunction::GEqual:
            glfunc = GL_GEQUAL;
            break;
        case DepthFunction::Always:
            glfunc = GL_ALWAYS;
            break;
        default:
            break;
    }
    if (glfunc)
        glDepthFunc(glfunc);
}

void gl::setPolygonMode(PolygonMode mode)
{
    GLenum glMode;
    switch (mode)
    {
        case PolygonMode::Triangle:
            glMode = GL_FILL;
            break;
        case PolygonMode::Wireframe:
            glMode = GL_LINE;
            break;
        case PolygonMode::Points:
            glMode = GL_POINT;
            break;
        default:
            return;
    }

    glPolygonMode(GL_FRONT_AND_BACK, glMode);
}

void gl::setFaceCull(FaceCull cull)
{
    switch (cull)
    {
        case FaceCull::All:
            glDisable(GL_CULL_FACE);
            break;
        case FaceCull::CW:
            glEnable(GL_CULL_FACE);
            glFrontFace(GL_CW);
            break;
        case FaceCull::CCW:
            glEnable(GL_CULL_FACE);
            glFrontFace(GL_CCW);
            break;
        default:
            break;
    }
}

void gl::setBlend(bool enabled)
{
    enabled ? glEnable(GL_BLEND) : glDisable(GL_BLEND);
}

void gl::setBlendFactor(BlendFactor srcFactor, BlendFactor dstFactor)
{
    glBlendFunc(toBlendFactor(srcFactor), toBlendFactor(dstFactor));
}

auto gl::toBlendFactor(BlendFactor factor) -> GLenum
{
    switch (factor)
    {
        case BlendFactor::Zero:
            return GL_ZERO;
        case BlendFactor::One:
            return GL_ONE;
        case BlendFactor::SrcColor:
            return GL_SRC_COLOR;
        case BlendFactor::OneMinusSrcColor:
            return GL_ONE_MINUS_SRC_COLOR;
        case BlendFactor::DstColor:
            return GL_DST_COLOR;
        case BlendFactor::OneMinusDstColor:
            return GL_ONE_MINUS_DST_COLOR;
        case BlendFactor::SrcAlpha:
            return GL_SRC_ALPHA;
        case BlendFactor::OneMinusSrcAlpha:
            return GL_ONE_MINUS_SRC_ALPHA;
        case BlendFactor::DstAlpha:
            return GL_DST_ALPHA;
        case BlendFactor::OneMinusDstAlpha:
            return GL_ONE_MINUS_DST_ALPHA;
        case BlendFactor::ConstantAlpha:
            return GL_CONSTANT_ALPHA;
        case BlendFactor::OneMinusConstantAlpha:
            return GL_ONE_MINUS_CONSTANT_ALPHA;
        case BlendFactor::SrcAlphaSaturate:
            return GL_SRC_ALPHA_SATURATE;
        default:
            SL_PANIC("Unknown blend factor");
            return 0;
    }
}

bool gl::findUniformInProgram(GLuint program, const char *name, GLint &location, int32_t &index)
{
    GLint activeUniforms;
    glGetProgramiv(program, GL_ACTIVE_UNIFORMS, &activeUniforms);
    if (activeUniforms <= 0)
        return false;

    GLint nameMaxLength;
    glGetProgramiv(program, GL_ACTIVE_UNIFORM_MAX_LENGTH, &nameMaxLength);
    if (nameMaxLength <= 0)
        return false;

    std::vector<GLchar> rawName(nameMaxLength + 1);
    uint32_t samplerIndex = 0;
    for (GLint i = 0; i < activeUniforms; ++i)
    {
        GLint size;
        GLenum type;

        glGetActiveUniform(program, i, nameMaxLength, nullptr, &size, &type, rawName.data());
        rawName[nameMaxLength] = '\0';
        std::string n = rawName.data();

        // Strip away possible square brackets for array uniforms,
        // they are sometimes present on some platforms
        auto bracketIndex = n.find('[');
        if (bracketIndex != std::string::npos)
            n.erase(bracketIndex);

        uint32_t idx = 0;
        if (type == GL_SAMPLER_2D || type == GL_SAMPLER_CUBE) // TODO other types of samplers
        {
            idx = samplerIndex;
            samplerIndex += size;
        }

        if (n == name)
        {
            location = glGetUniformLocation(program, rawName.data());
            index = idx;
            return true;
        }
    }

    return false;
}

#endif
