/*
    Copyright (c) Aleksey Fedotov
    MIT license
*/

#include "SoloStbRectTextureData.h"
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
            SL_PANIC("Unsupported components count ", SL_FMT(components))
            return TextureFormat::RGB;
    }
}

bool stb::RectTextureData::canLoadFromFile(const std::string &path)
{
    static std::vector<std::string> supportedFormats = {".bmp", ".jpg", ".jpeg", ".png"};
    return std::find_if(supportedFormats.begin(), supportedFormats.end(),
        [&](const std::string &ext) { return stringutils::endsWith(path, ext); }) != supportedFormats.end();
}

stb::RectTextureData::~RectTextureData()
{
    if (data)
        stbi_image_free(data);
}

auto stb::RectTextureData::loadFromFile(Device *device, const std::string &path) -> sptr<RectTextureData>
{
    auto bytes = device->getFileSystem()->readBytes(path);
    int width, height, bpp;
    stbi_set_flip_vertically_on_load(device->getSetup().mode == DeviceMode::OpenGL);
    const auto data = stbi_load_from_memory(bytes.data(), bytes.size(), &width, &height, &bpp, 0);
    SL_PANIC_IF(!data, "Failed to load image ", path);

    const auto result = std::make_shared<RectTextureData>();
    result->bpp = bpp;
    result->format = toImageFormat(bpp);
    result->width = width;
    result->height = height;
    result->data = data;
    return result;
}
