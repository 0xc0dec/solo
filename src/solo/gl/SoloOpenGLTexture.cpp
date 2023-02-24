/*
 * Copyright (c) Aleksey Fedotov
 * MIT license
 */

#include "SoloOpenGLTexture.h"

#ifdef SL_OPENGL_RENDERER

#include "SoloTextureData.h"
#include "SoloHash.h"

using namespace solo;

static auto getFormatKey(GLenum internalFormat, GLenum format, GLenum type) -> size_t {
    size_t seed = 0;
    const std::hash<GLenum> hasher;
    combineHash(seed, hasher(internalFormat));
    combineHash(seed, hasher(format));
    combineHash(seed, hasher(type));
    return seed;
}

static void detectFormatSupport(GLenum internalFormat, GLenum format, GLenum type, uset<size_t> &supportedFormats) {
    glTexImage2D(GL_PROXY_TEXTURE_2D, 0, internalFormat, 32, 32, 0, format, type, nullptr);
    s32 width;
    glGetTexLevelParameteriv(GL_PROXY_TEXTURE_2D, 0, GL_TEXTURE_WIDTH, &width);
    if (width == 32)
        supportedFormats.insert(getFormatKey(internalFormat, format, type));
}

static bool isFormatSupported(GLenum internalFormat, GLenum format, GLenum type) {
    static uset<size_t> supportedFormats;
    static auto initialized = false;

    if (!initialized) {
        detectFormatSupport(GL_R8, GL_RED, GL_UNSIGNED_BYTE, supportedFormats);

        detectFormatSupport(GL_RGB8, GL_RGB, GL_UNSIGNED_BYTE, supportedFormats);
        detectFormatSupport(GL_RGB8, GL_RGBA, GL_UNSIGNED_BYTE, supportedFormats);

        detectFormatSupport(GL_RGBA8, GL_RGB, GL_UNSIGNED_BYTE, supportedFormats);
        detectFormatSupport(GL_RGBA8, GL_RGBA, GL_UNSIGNED_BYTE, supportedFormats);

        detectFormatSupport(GL_RGBA16F, GL_RGB, GL_UNSIGNED_BYTE, supportedFormats);
        detectFormatSupport(GL_RGBA16F, GL_RGBA, GL_UNSIGNED_BYTE, supportedFormats);
        detectFormatSupport(GL_RGBA16F, GL_RGB, GL_FLOAT, supportedFormats);
        detectFormatSupport(GL_RGBA16F, GL_RGBA, GL_FLOAT, supportedFormats);

        detectFormatSupport(GL_DEPTH_COMPONENT24, GL_DEPTH_COMPONENT, GL_UNSIGNED_BYTE, supportedFormats);
        detectFormatSupport(GL_DEPTH_COMPONENT24, GL_DEPTH_COMPONENT, GL_FLOAT, supportedFormats);

        initialized = true;
    }

    return supportedFormats.count(getFormatKey(internalFormat, format, type));
}

static auto toType(TextureFormat format) -> GLenum {
    switch (format) {
    case TextureFormat::Depth24:
        return GL_FLOAT;
    default:
        return GL_UNSIGNED_BYTE;
    }
}

static auto toDataFormat(TextureDataFormat format) -> GLenum {
    switch (format) {
    case TextureDataFormat::Red:
        return GL_RED;
    case TextureDataFormat::RGB:
        return GL_RGB;
    case TextureDataFormat::RGBA:
        return GL_RGBA;
    }

    panic("Unsupported texture data format");

    return 0;
}

static auto toDataFormat(TextureFormat format) -> GLenum {
    switch (format) {
    case TextureFormat::R8:
        return GL_RED;
    case TextureFormat::RGB8:
        return GL_RGB;
    case TextureFormat::RGBA8:
    case TextureFormat::RGBA16F:
        return GL_RGBA;
    case TextureFormat::Depth24:
        return GL_DEPTH_COMPONENT;
    }

    panic("Unsupported texture format");

    return 0;
}

static auto toInternalFormat(TextureFormat format) -> GLenum {
    switch (format) {
    case TextureFormat::R8:
        return GL_R8;
    case TextureFormat::RGB8:
        return GL_RGB8;
    case TextureFormat::RGBA8:
        return GL_RGBA8;
    case TextureFormat::RGBA16F:
        return GL_RGBA16F;
    case TextureFormat::Depth24:
        return GL_DEPTH_COMPONENT24;
    }

    panic("Unsupported texture format");

    return 0;
}

static auto toWrap(TextureWrap wrap) -> GLenum {
    switch (wrap) {
    case TextureWrap::MirrorRepeat:
        return GL_MIRRORED_REPEAT;
    case TextureWrap::ClampToEdge:
        return GL_CLAMP_TO_EDGE;
    case TextureWrap::ClampToBorder:
        return GL_CLAMP_TO_BORDER;
    case TextureWrap::Repeat:
        return GL_REPEAT;
    }

    panic("Unsupported wrap mode");

    return 0;
}

static auto toMinFilter(TextureFilter minFilter, TextureMipFilter mipFilter) -> GLenum {
    switch (minFilter) {
    case TextureFilter::Linear: {
            switch (mipFilter) {
            case TextureMipFilter::Linear:
                return GL_LINEAR_MIPMAP_LINEAR;
            case TextureMipFilter::Nearest:
                return GL_LINEAR_MIPMAP_NEAREST;
            case TextureMipFilter::None:
                return GL_LINEAR;
            default:
                panic("Unsupported mip filter");
                return 0;
            }
        }

    case TextureFilter::Nearest: {
            switch (mipFilter) {
            case TextureMipFilter::Linear:
                return GL_NEAREST_MIPMAP_LINEAR;
            case TextureMipFilter::Nearest:
                return GL_NEAREST_MIPMAP_NEAREST;
            case TextureMipFilter::None:
                return GL_NEAREST;
            default:
                panic("Unsupported mip filter");
                return 0;
            }
        }
    default:
        panic("Unsupported min filter");
        return 0;
    }
}

static auto toMagFilter(TextureFilter filter) -> GLenum {
    switch (filter) {
    case TextureFilter::Linear:
        return GL_LINEAR;
    case TextureFilter::Nearest:
        return GL_NEAREST;
    }

    panic("Unsupported mag filter");

    return 0;
}

OpenGLTexture::OpenGLTexture() {
    glGenTextures(1, &handle_);
    panicIf(!handle_, "Unable to create texture handle");
}

OpenGLTexture::~OpenGLTexture() {
    glDeleteTextures(1, &handle_);
}

OpenGLTexture2D::OpenGLTexture2D(TextureFormat format, Vector2 dimensions):
    Texture2D(format, dimensions) {
}

auto OpenGLTexture2D::fromData(sptr<Texture2DData> data, bool generateMipmaps) -> sptr<OpenGLTexture2D> {
    const auto dimensions = data->dimensions();
    const auto internalFormat = toInternalFormat(data->textureFormat());
    const auto dataFormat = toDataFormat(data->format());
    const auto mipLevels = generateMipmaps
    ? std::floor(std::log2((std::max)(dimensions.x(), dimensions.y()))) + 1
    : 0;

    panicIf(!isFormatSupported(internalFormat, dataFormat, GL_UNSIGNED_BYTE), "Texture format not supported");

    const auto result = sptr<OpenGLTexture2D>(new OpenGLTexture2D(data->textureFormat(), dimensions));

    glBindTexture(GL_TEXTURE_2D, result->handle_);

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, mipLevels);
    glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, dimensions.x(), dimensions.y(), 0, dataFormat, GL_UNSIGNED_BYTE, data->data());

    if (generateMipmaps && data->textureFormat() != TextureFormat::Depth24) {
        glHint(GL_GENERATE_MIPMAP_HINT, GL_NICEST);
        glGenerateMipmap(GL_TEXTURE_2D);
    }

    glBindTexture(GL_TEXTURE_2D, 0);

    return result;
}

auto OpenGLTexture2D::empty(u32 width, u32 height, TextureFormat format) -> sptr<OpenGLTexture2D> {
    const auto internalFormat = toInternalFormat(format);
    const auto type = toType(format);
    const auto dataFormat = toDataFormat(format);
    const auto dimensions = Vector2(width, height);

    panicIf(!isFormatSupported(internalFormat, dataFormat, GL_UNSIGNED_BYTE), "Texture format not supported");

    const auto result = sptr<OpenGLTexture2D>(new OpenGLTexture2D(format, dimensions));

    glBindTexture(GL_TEXTURE_2D, result->handle_);

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, 0);
    glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, dimensions.x(), dimensions.y(), 0, dataFormat, type, nullptr);

    glBindTexture(GL_TEXTURE_2D, 0);

    return result;
}

void OpenGLTexture2D::bind() {
    glBindTexture(GL_TEXTURE_2D, handle_);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, toMinFilter(minFilter_, mipFilter_));
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, toMagFilter(magFilter_));
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, toWrap(horizontalWrap_));
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, toWrap(verticalWrap_));

    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, anisotropyLevel_);
}

auto OpenGLCubeTexture::fromData(sptr<CubeTextureData> data) -> sptr<OpenGLCubeTexture> {
    const auto result = sptr<OpenGLCubeTexture>(new OpenGLCubeTexture(data->textureFormat(), data->dimension()));

    glBindTexture(GL_TEXTURE_CUBE_MAP, result->handle_);

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAX_LEVEL, 0);

    for (s32 i = 0; i < 6; ++i) {
        const auto glFace = static_cast<u32>(GL_TEXTURE_CUBE_MAP_POSITIVE_X) + i;
        const auto internalFormat = toInternalFormat(data->textureFormat());
        const auto dataFormat = toDataFormat(data->format());
        panicIf(!isFormatSupported(internalFormat, dataFormat, GL_UNSIGNED_BYTE), "Texture format not supported");
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
        glTexImage2D(glFace, 0, internalFormat, data->dimension(), data->dimension(), 0, dataFormat, GL_UNSIGNED_BYTE, data->faceData(i));
    }

    glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

    return result;
}

OpenGLCubeTexture::OpenGLCubeTexture(TextureFormat format, u32 dimension):
    CubeTexture(format, dimension) {
}

void OpenGLCubeTexture::bind() {
    glBindTexture(GL_TEXTURE_CUBE_MAP, handle_);

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, toMinFilter(minFilter_, mipFilter_));
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, toMagFilter(magFilter_));
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, toWrap(horizontalWrap_));
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, toWrap(verticalWrap_));
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, toWrap(depthWrap_));

    glTexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAX_ANISOTROPY_EXT, anisotropyLevel_);
}

#endif
