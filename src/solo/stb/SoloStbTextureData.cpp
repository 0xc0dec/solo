/*
    Copyright (c) Aleksey Fedotov
    MIT license
*/

#include "SoloStbTextureData.h"
#include "SoloStringUtils.h"
#include "SoloDevice.h"
#include "SoloFileSystem.h"
#include <vector>
#include <algorithm>
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

using namespace solo;

static auto toImageFormat(int components) -> TextureFormat
{
    switch (components)
    {
        case 1: return TextureFormat::Red; // TODO Not sure about this
        case 3: return TextureFormat::RGB;
        case 4: return TextureFormat::RGBA;
        default:
            SL_PANIC(SL_FMT("Unsupported components count ", components))
            return TextureFormat::RGB;
    }
}

bool stb::Texture2dData::canLoadFromFile(const std::string &path)
{
    static std::vector<std::string> supportedFormats = {".bmp", ".jpg", ".jpeg", ".png"};
    return std::find_if(supportedFormats.begin(), supportedFormats.end(),
        [&](const std::string &ext) { return stringutils::endsWith(path, ext); }) != supportedFormats.end();
}

stb::Texture2dData::~Texture2dData()
{
    if (data)
        stbi_image_free(data);
}

auto stb::Texture2dData::loadFromFile(Device *device, const std::string &path) -> sptr<Texture2dData>
{
    auto bytes = device->getFileSystem()->readBytes(path);
    int width, height, bpp;
    stbi_set_flip_vertically_on_load(device->getSetup().mode == DeviceMode::OpenGL);
    const auto data = stbi_load_from_memory(bytes.data(), bytes.size(), &width, &height, &bpp, 0);
    SL_PANIC_IF(!data, SL_FMT("Failed to load image ", path));

    const auto result = std::make_shared<Texture2dData>();
    result->bpp = bpp;
    result->format = toImageFormat(bpp);
    result->width = width;
    result->height = height;
    result->data = data;
    return result;
}

bool stb::CubeTextureData::canLoadFromFaceFiles(
    const std::string &frontPath,
    const std::string &backPath,
    const std::string &leftPath,
    const std::string &rightPath,
    const std::string &topPath,
    const std::string &bottomPath)
{
    return stb::Texture2dData::canLoadFromFile(frontPath) &&
           stb::Texture2dData::canLoadFromFile(backPath) &&
           stb::Texture2dData::canLoadFromFile(leftPath) &&
           stb::Texture2dData::canLoadFromFile(rightPath) &&
           stb::Texture2dData::canLoadFromFile(topPath) &&
           stb::Texture2dData::canLoadFromFile(bottomPath);
}

auto stb::CubeTextureData::loadFromFaceFiles(Device *device,
    const std::string &frontPath,
    const std::string &backPath,
    const std::string &leftPath,
    const std::string &rightPath,
    const std::string &topPath,
    const std::string &bottomPath) -> sptr<CubeTextureData>
{
    auto tex = std::make_shared<stb::CubeTextureData>();
    tex->faces.push_back(stb::Texture2dData::loadFromFile(device, frontPath));
    tex->faces.push_back(stb::Texture2dData::loadFromFile(device, backPath));
    tex->faces.push_back(stb::Texture2dData::loadFromFile(device, leftPath));
    tex->faces.push_back(stb::Texture2dData::loadFromFile(device, rightPath));
    tex->faces.push_back(stb::Texture2dData::loadFromFile(device, topPath));
    tex->faces.push_back(stb::Texture2dData::loadFromFile(device, bottomPath));
    
    SL_PANIC_BLOCK(
    {
        auto width = tex->faces[0]->getWidth();
        auto height = tex->faces[0]->getHeight();
        SL_PANIC_IF(width != height, "Cube texture width must be equal to height");
        for (const auto &face: tex->faces)
            SL_PANIC_IF(face->getWidth() != width || face->getHeight() != height, "All cube texture sizes must match");
    });

    return tex;
}

auto stb::CubeTextureData::getMipLevels() const -> uint32_t
{
    return 1;
}

auto stb::CubeTextureData::getSize() const -> uint32_t
{
    return faces[0]->getSize() + 
           faces[1]->getSize() + 
           faces[2]->getSize() + 
           faces[3]->getSize() + 
           faces[4]->getSize() + 
           faces[5]->getSize();
}

auto stb::CubeTextureData::getSize(uint32_t mipLevel) const -> uint32_t
{
    return faces[0]->getSize(mipLevel) + 
           faces[1]->getSize(mipLevel) + 
           faces[2]->getSize(mipLevel) + 
           faces[3]->getSize(mipLevel) + 
           faces[4]->getSize(mipLevel) + 
           faces[5]->getSize(mipLevel);
}

auto stb::CubeTextureData::getSize(uint32_t face, uint32_t mipLevel) const -> uint32_t
{
    return faces[face]->getSize(mipLevel);
}

auto stb::CubeTextureData::getDimension() const -> uint32_t
{
    return faces[0]->getWidth();
}

auto stb::CubeTextureData::getDimension(unsigned mipLevel) const -> uint32_t
{
    return faces[0]->getWidth(mipLevel);
}

auto stb::CubeTextureData::getData() const -> const void*
{
    SL_PANIC("Not implemented");
    return nullptr;
}

auto stb::CubeTextureData::getData(uint32_t face) const -> const void*
{
    return faces[face]->getData();
}

auto stb::CubeTextureData::getFormat() const -> TextureFormat
{
    return faces[0]->getFormat();
}