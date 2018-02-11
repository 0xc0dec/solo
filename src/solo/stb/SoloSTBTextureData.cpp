/*
    Copyright (c) Aleksey Fedotov
    MIT license
*/

#include "SoloSTBTextureData.h"
#include "SoloStringUtils.h"
#include "SoloDevice.h"
#include "SoloFileSystem.h"
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
			return panic<TextureFormat>(SL_FMT("Unsupported components count ", components));
    }
}

bool STBTexture2dData::canLoadFromFile(const str &path)
{
    static vec<str> supportedFormats = {".bmp", ".jpg", ".jpeg", ".png"};
    return std::find_if(supportedFormats.begin(), supportedFormats.end(),
        [&](const str &ext) { return stringutils::endsWith(path, ext); }) != supportedFormats.end();
}

STBTexture2dData::~STBTexture2dData()
{
    if (data)
        stbi_image_free(data);
}

auto STBTexture2dData::loadFromFile(Device *device, const str &path) -> sptr<STBTexture2dData>
{
    auto bytes = device->getFileSystem()->readBytes(path);
    int width, height, channels;
    stbi_set_flip_vertically_on_load(device->getMode() == DeviceMode::OpenGL);
    // According to the docs, channels are not affected by the requested channels
    const auto data = stbi_load_from_memory(bytes.data(), bytes.size(), &width, &height, &channels, 4);
    panicIf(!data, SL_FMT("Failed to load image ", path));

    const auto result = std::make_shared<STBTexture2dData>();
    result->channels = 4;
    result->format = toImageFormat(4);
    result->width = width;
    result->height = height;
    result->data = data;
    return result;
}

bool STBCubeTextureData::canLoadFromFaceFiles(
	const str& positiveXPath, const str& negativeXPath,
	const str& positiveYPath, const str& negativeYPath,
	const str& positiveZPath, const str& negativeZPath)
{
	return STBTexture2dData::canLoadFromFile(positiveXPath) &&
		STBTexture2dData::canLoadFromFile(negativeXPath) &&
		STBTexture2dData::canLoadFromFile(positiveYPath) &&
		STBTexture2dData::canLoadFromFile(negativeYPath) &&
		STBTexture2dData::canLoadFromFile(positiveZPath) &&
		STBTexture2dData::canLoadFromFile(negativeZPath);
}

auto STBCubeTextureData::loadFromFaceFiles(
    Device *device,
    const str& positiveXPath, const str& negativeXPath,
	const str& positiveYPath, const str& negativeYPath,
	const str& positiveZPath, const str& negativeZPath) -> sptr<STBCubeTextureData>
{
    auto tex = std::make_shared<STBCubeTextureData>();
    tex->faces.push_back(STBTexture2dData::loadFromFile(device, positiveXPath));
    tex->faces.push_back(STBTexture2dData::loadFromFile(device, negativeXPath));
    tex->faces.push_back(STBTexture2dData::loadFromFile(device, positiveYPath));
    tex->faces.push_back(STBTexture2dData::loadFromFile(device, negativeYPath));
    tex->faces.push_back(STBTexture2dData::loadFromFile(device, positiveZPath));
    tex->faces.push_back(STBTexture2dData::loadFromFile(device, negativeZPath));
    
    SL_DEBUG_BLOCK(
    {
        const auto width = tex->faces[0]->getWidth();
        const auto height = tex->faces[0]->getHeight();
        panicIf(width != height, "Cube texture width must be equal to height");
        for (const auto &face: tex->faces)
            panicIf(face->getWidth() != width || face->getHeight() != height, "All cube texture sizes must match");
    });

    return tex;
}

auto STBCubeTextureData::getSize() const -> u32
{
    return faces[0]->getSize() + 
           faces[1]->getSize() + 
           faces[2]->getSize() + 
           faces[3]->getSize() + 
           faces[4]->getSize() + 
           faces[5]->getSize();
}

auto STBCubeTextureData::getSize(u32 face) const -> u32
{
    return faces[face]->getSize();
}

auto STBCubeTextureData::getDimension() const -> u32
{
    return faces[0]->getWidth();
}

auto STBCubeTextureData::getData(u32 face) const -> const void*
{
    return faces[face]->getData();
}

auto STBCubeTextureData::getFormat() const -> TextureFormat
{
    return faces[0]->getFormat();
}