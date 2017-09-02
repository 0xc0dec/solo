/*
    Copyright (c) Aleksey Fedotov
    MIT license
*/

#include "SoloTexture.h"
#include "SoloPngImageLoader.h"
#include "SoloImage.h"
#include "SoloRectTexture.h"
#include "SoloCubeTexture.h"
#include <vector>

using namespace solo;

// TODO remove later when proper loading is implemented
static std::vector<sptr<ImageLoader>> imageLoaders;

static void initImageLoaders(Device *device)
{
    if (!imageLoaders.empty())
        return;
    imageLoaders.push_back(std::make_unique<PngImageLoader>(device));
}

static auto getImageLoader(const std::string &path) -> ImageLoader*
{
    for (const auto &l : imageLoaders)
    {
        if (l->isLoadable(path))
            return l.get();
    }
    SL_PANIC(SL_FMT("No suitable loader found for image '", path, "'"));
    return nullptr;
}

Texture::Texture()
{
    rebuildFlags(); // yes, virtual call
}

auto Texture::loadRectFromFile(Device *device, const std::string &path) -> sptr<RectTexture>
{
    initImageLoaders(device);
    auto loader = getImageLoader(path);
    auto image = loader->load(path);
    auto result = RectTexture::create(device);
    result->setData(image->format, image->data.data(), image->width, image->height);
    return result;
}

auto Texture::loadCubeFromFiles(Device *device, const std::vector<std::string> &paths) -> sptr<CubeTexture>
{
    initImageLoaders(device);

    auto result = CubeTexture::create(device);
    auto loader = getImageLoader(paths[0]);

    auto idx = 0;
    for (const auto &path : paths)
    {
        auto image = loader->load(path);
        const auto face = static_cast<CubeTextureFace>(static_cast<uint32_t>(CubeTextureFace::Front) + idx++);
        result->setData(face, image->format, image->data.data(), image->width, image->height);
    }

    return result;
}

void Texture::setWrapping(TextureWrapping wrap)
{
    horizontalWrapping = wrap;
    verticalWrapping = wrap;
    rebuildFlags();
}

void Texture::rebuildFlags()
{
    flags = 0;

    switch (horizontalWrapping)
    {
        case TextureWrapping::Clamp:
            flags |= TextureFlags::HorizontalWrapClamp;
            break;
        case TextureWrapping::Repeat:
            flags |= TextureFlags::HorizontalWrapRepeat;
            break;
        default:
            break;
    }

    switch (verticalWrapping)
    {
        case TextureWrapping::Clamp:
            flags |= TextureFlags::VerticalWrapClamp;
            break;
        case TextureWrapping::Repeat:
            flags |= TextureFlags::VerticalWrapRepeat;
            break;
        default:
            break;
    }

    switch (minFiltering)
    {
        case TextureFiltering::Nearest:
            flags |= TextureFlags::MinFilterNearest;
            break;
        case TextureFiltering::Linear:
            flags |= TextureFlags::MinFilterLinear;
            break;
        case TextureFiltering::NearestMipmapNearest:
            flags |= TextureFlags::MinFilterNearestMipmapNearest;
            break;
        case TextureFiltering::LinearMipmapNearest:
            flags |= TextureFlags::MinFilterLinearMipmapNearest;
            break;
        case TextureFiltering::NearestMipmapLinear:
            flags |= TextureFlags::MinFilterNearestMipmapLinear;
            break;
        case TextureFiltering::LinearMipmapLinear:
            flags |= TextureFlags::MinFilterLinearMipmapLinear;
            break;
        default:
            break;
    }

    switch (magFiltering)
    {
        case TextureFiltering::Nearest:
            flags |= TextureFlags::MagFilterNearest;
            break;
        case TextureFiltering::Linear:
            flags |= TextureFlags::MagFilterLinear;
            break;
        case TextureFiltering::NearestMipmapNearest:
            flags |= TextureFlags::MagFilterNearestMipmapNearest;
            break;
        case TextureFiltering::LinearMipmapNearest:
            flags |= TextureFlags::MagFilterLinearMipmapNearest;
            break;
        case TextureFiltering::NearestMipmapLinear:
            flags |= TextureFlags::MagFilterNearestMipmapLinear;
            break;
        case TextureFiltering::LinearMipmapLinear:
            flags |= TextureFlags::MagFilterLinearMipmapLinear;
            break;
        default:
            break;
    }
}

