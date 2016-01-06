#include "SoloOpenGLHelper.h"

using namespace solo;


GLenum OpenGLHelper::convertToGLColorFormat(ColorFormat format)
{
    switch (format)
    {
    case ColorFormat::RGB:
        return GL_RGB;
    case ColorFormat::RGBA:
        return GL_RGBA;
    default:
        SL_THROW_FMT(EngineException, "Unexpected texture format ", static_cast<int>(format));
    }
}


GLenum OpenGLHelper::convertToGLWrapMode(TextureWrapping mode)
{
    switch (mode)
    {
    case TextureWrapping::Clamp:
        return GL_CLAMP_TO_EDGE;
    case TextureWrapping::Repeat:
        return GL_REPEAT;
    default:
        SL_THROW_FMT(EngineException, "Unexpected wrap mode ", static_cast<int>(mode));
    }
}


GLenum OpenGLHelper::convertToGLFilter(TextureFiltering filter)
{
    switch (filter)
    {
    case TextureFiltering::Linear:
        return GL_LINEAR;
    case TextureFiltering::Nearest:
        return GL_NEAREST;
    case TextureFiltering::LinearMipmapNearest:
        return GL_LINEAR_MIPMAP_NEAREST;
    case TextureFiltering::LinearMipmapLinear:
        return GL_LINEAR_MIPMAP_LINEAR;
    case TextureFiltering::NearestMipmapLinear:
        return GL_NEAREST_MIPMAP_LINEAR;
    case TextureFiltering::NearestMipmapNearest:
        return GL_NEAREST_MIPMAP_NEAREST;
    default:
        SL_THROW_FMT(EngineException, "Unexpected texture filter ", static_cast<int>(filter));
    }
}