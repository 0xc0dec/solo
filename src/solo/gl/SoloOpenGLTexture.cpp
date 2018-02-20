/*
    Copyright (c) Aleksey Fedotov
    MIT license
*/

#include "SoloOpenGLTexture.h"

#ifdef SL_OPENGL_RENDERER

#include "SoloTextureData.h"

using namespace solo;

static auto toType(TextureFormat format) -> GLenum
{
    switch (format)
    {
        case TextureFormat::Depth:
            return GL_FLOAT;
        default:
            return GL_UNSIGNED_BYTE;
    }
}

static auto toFormat(TextureFormat format) -> GLenum
{
    switch (format)
    {
        case TextureFormat::Red:
            return GL_RED;
        case TextureFormat::RGB:
            return GL_RGB;
        case TextureFormat::RGBA:
            return GL_RGBA;
        case TextureFormat::Depth:
            return GL_DEPTH_COMPONENT;
        default:
            return panic<GLenum>("Unknown image format");
    }
}

static auto toInternalFormat(TextureFormat format) -> GLenum
{
    switch (format)
    {
        case TextureFormat::Red:
        case TextureFormat::RGB:
            return GL_RGB;
        case TextureFormat::RGBA:
            return GL_RGBA;
        case TextureFormat::Depth:
            return GL_DEPTH_COMPONENT24;
        default:
            return panic<GLenum>("Unknown image format");
    }
}

static auto toWrap(TextureWrap wrap) -> GLenum
{
    switch (wrap)
    {
        case TextureWrap::MirrorRepeat:
            return GL_MIRRORED_REPEAT;
        case TextureWrap::ClampToEdge:
            return GL_CLAMP_TO_EDGE;
        case TextureWrap::ClampToBorder:
            return GL_CLAMP_TO_BORDER;
        case TextureWrap::Repeat:
            return GL_REPEAT;
        default:
            return panic<GLenum>("Unsupported wrap mode");
    }
}

static auto toMinFilter(TextureFilter minFilter, TextureMipFilter mipFilter) -> GLenum
{
    switch (minFilter)
    {
        case TextureFilter::Linear:
        {
            switch (mipFilter)
            {
                case TextureMipFilter::Linear: return GL_LINEAR_MIPMAP_LINEAR;
                case TextureMipFilter::Nearest: return GL_LINEAR_MIPMAP_NEAREST;
                case TextureMipFilter::None: return GL_LINEAR;
                default:
                    return panic<GLenum>("Unsupported mip filter");
            }
            break;
        }

        case TextureFilter::Nearest:
        {
            switch (mipFilter)
            {
                case TextureMipFilter::Linear: return GL_NEAREST_MIPMAP_LINEAR;
                case TextureMipFilter::Nearest: return GL_NEAREST_MIPMAP_NEAREST;
                case TextureMipFilter::None: return GL_NEAREST;
                default:
                    return panic<GLenum>("Unsupported mip filter");
            }
        }
        default:
            return panic<GLenum>("Unsupported min filter");
    }
}

static auto toMagFilter(TextureFilter filter) -> GLenum
{
    switch (filter)
    {
        case TextureFilter::Linear: return GL_LINEAR;
        case TextureFilter::Nearest: return GL_NEAREST;
        default:
            return panic<GLenum>("Unsupported mag filter");
    }
}

OpenGLTexture::OpenGLTexture()
{
    glGenTextures(1, &handle);
    panicIf(!handle, "Failed to create texture handle");
}

OpenGLTexture::~OpenGLTexture()
{
    glDeleteTextures(1, &handle);
}

OpenGLTexture2D::OpenGLTexture2D(TextureFormat format, Vector2 dimensions):
    Texture2D(format, dimensions)
{
}

auto OpenGLTexture2D::createFromData(Texture2DData *data, bool generateMipmaps) -> sptr<OpenGLTexture2D>
{
    const auto dimensions = data->getDimensions();
    const auto internalFormat = toInternalFormat(data->getFormat());
    const auto format = toFormat(data->getFormat());
    const auto type = toType(data->getFormat());
    const auto mipLevels = generateMipmaps
        ? std::floor(std::log2((std::max)(dimensions.x(), dimensions.y()))) + 1
        : 0; // TODO remove this copy-paste (from VK as well)

    const auto result = sptr<OpenGLTexture2D>(new OpenGLTexture2D(data->getFormat(), dimensions));

    glBindTexture(GL_TEXTURE_2D, result->handle);

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, mipLevels);
    glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, dimensions.x(), dimensions.y(), 0, format, type, data->getData());

    if (generateMipmaps && data->getFormat() != TextureFormat::Depth)
    {
        glHint(GL_GENERATE_MIPMAP_HINT, GL_NICEST);
        glGenerateMipmap(GL_TEXTURE_2D);
    }

    glBindTexture(GL_TEXTURE_2D, 0);

    return result;
}

void OpenGLTexture2D::bind()
{
    glBindTexture(GL_TEXTURE_2D, handle);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, toMinFilter(minFilter, mipFilter));
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, toMagFilter(magFilter));
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, toWrap(horizontalWrap));
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, toWrap(verticalWrap));

    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, anisotropyLevel);
}

auto OpenGLCubeTexture::createFromData(CubeTextureData *data) -> sptr<OpenGLCubeTexture>
{
    const auto result = sptr<OpenGLCubeTexture>(new OpenGLCubeTexture(data->getFormat(), data->getDimension()));

    glBindTexture(GL_TEXTURE_CUBE_MAP, result->handle);

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAX_LEVEL, 0);

    for (s32 i = 0; i < 6; ++i)
    {
        const auto glFace = static_cast<u32>(GL_TEXTURE_CUBE_MAP_POSITIVE_X) + i;
        const auto internalFormat = toInternalFormat(data->getFormat());
        const auto fmt = toFormat(data->getFormat());
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
        glTexImage2D(glFace, 0, internalFormat, data->getDimension(), data->getDimension(), 0, fmt, GL_UNSIGNED_BYTE, data->getData(i));
    }

    glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

    return result;
}

OpenGLCubeTexture::OpenGLCubeTexture(TextureFormat format, u32 dimension):
    CubeTexture(format, dimension)
{
}

void OpenGLCubeTexture::bind()
{
    glBindTexture(GL_TEXTURE_CUBE_MAP, handle);

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, toMinFilter(minFilter, mipFilter));
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, toMagFilter(magFilter));
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, toWrap(horizontalWrap));
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, toWrap(verticalWrap));
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, toWrap(depthWrap));

    glTexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAX_ANISOTROPY_EXT, anisotropyLevel);
}

#endif
