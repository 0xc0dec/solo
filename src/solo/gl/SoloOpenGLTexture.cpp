/*
    Copyright (c) Aleksey Fedotov
    MIT license
*/

#include "SoloOpenGLTexture.h"

#ifdef SL_OPENGL_RENDERER

#include "SoloTextureData.h"

using namespace solo;

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
        default:
            SL_PANIC("Unknown image format");
            return GL_RED;
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
        default:
            SL_PANIC("Unknown image format");
            return GL_RGB;
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
            SL_PANIC("Unsupported wrap mode");
            return GL_REPEAT;
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
                    SL_PANIC("Unsupported mip filter")
                    return GL_LINEAR;
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
                    SL_PANIC("Unsupported mip filter")
                    return GL_LINEAR;
            }
            break;
        }
        default:
            SL_PANIC("Unsupported min filter")
            return GL_LINEAR;
    }
}

static auto toMagFilter(TextureFilter filter) -> GLenum
{
    switch (filter)
    {
        case TextureFilter::Linear: return GL_LINEAR;
        case TextureFilter::Nearest: return GL_NEAREST;
        default:
            SL_PANIC("Unsupported mag filter")
            return GL_LINEAR;
    }
}

static auto toCubeMapFace(CubeTextureFace face) -> GLenum
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

OpenGLTexture::OpenGLTexture()
{
    glGenTextures(1, &handle);
    SL_PANIC_IF(!handle, "Failed to create texture handle");
}

OpenGLTexture::~OpenGLTexture()
{
    glDeleteTextures(1, &handle);
}

OpenGLTexture2d::OpenGLTexture2d(Texture2dData *data, bool generateMipmaps):
    Texture2d(data)
{
    setData(data->getData(), generateMipmaps);
}

void OpenGLTexture2d::bind()
{
    glBindTexture(GL_TEXTURE_2D, handle);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, toMinFilter(minFilter, mipFilter));
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, toMagFilter(magFilter));
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, toWrap(horizontalWrap));
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, toWrap(verticalWrap));

    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, anisotropyLevel);
}

void OpenGLTexture2d::setData(const void *data, bool generateMipmaps)
{
    const auto internalFormat = toInternalFormat(this->format);
    const auto format = toFormat(this->format);
    const auto mipLevels = generateMipmaps ? floor(log2((std::max)(dimensions.x, dimensions.y))) + 1 : 0; // TODO remove this copy-paste (from VK as well)

    glBindTexture(GL_TEXTURE_2D, handle);

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, mipLevels);
    glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, dimensions.x, dimensions.y, 0, format, GL_UNSIGNED_BYTE, data);

    if (generateMipmaps)
    {
        glHint(GL_GENERATE_MIPMAP_HINT, GL_NICEST);
        glGenerateMipmap(GL_TEXTURE_2D);
    }

    glBindTexture(GL_TEXTURE_2D, 0);
}

OpenGLCubeTexture::OpenGLCubeTexture(CubeTextureData *data, bool generateMipmaps):
    CubeTexture(data)
{
    setData(data, generateMipmaps);
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

void OpenGLCubeTexture::setData(CubeTextureData *data, bool generateMipmaps)
{
    const auto mipLevels = generateMipmaps ? floor(log2((std::max)(dimension, dimension))) + 1 : 0; // TODO remove this copy-paste (from VK as well)

    glBindTexture(GL_TEXTURE_CUBE_MAP, handle);

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAX_LEVEL, mipLevels);

    for (s32 i = 0; i < 6; ++i)
    {
        const auto face = static_cast<CubeTextureFace>(static_cast<u32>(CubeTextureFace::Front) + i);
        const auto glFace = toCubeMapFace(face);
        const auto internalFormat = toInternalFormat(data->getFormat());
        const auto fmt = toFormat(data->getFormat());
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
        glTexImage2D(glFace, 0, internalFormat, data->getDimension(), data->getDimension(), 0, fmt, GL_UNSIGNED_BYTE, data->getData(i));
    }

    if (generateMipmaps)
    {
        glHint(GL_GENERATE_MIPMAP_HINT, GL_NICEST);
        glGenerateMipmap(GL_TEXTURE_CUBE_MAP);
    }

    glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
}

#endif
