/*
    Copyright (c) Aleksey Fedotov
    MIT license
*/

#include "SoloStbCubeTextureData.h"
#include "SoloDevice.h"
#include <vector>

using namespace solo;

bool stb::CubeTextureData::canLoadFromFaceFiles(
    const std::string &frontPath,
    const std::string &backPath,
    const std::string &leftPath,
    const std::string &rightPath,
    const std::string &topPath,
    const std::string &bottomPath)
{
    return stb::RectTextureData::canLoadFromFile(frontPath) &&
           stb::RectTextureData::canLoadFromFile(backPath) &&
           stb::RectTextureData::canLoadFromFile(leftPath) &&
           stb::RectTextureData::canLoadFromFile(rightPath) &&
           stb::RectTextureData::canLoadFromFile(topPath) &&
           stb::RectTextureData::canLoadFromFile(bottomPath);
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
    tex->faces.push_back(stb::RectTextureData::loadFromFile(device, frontPath));
    tex->faces.push_back(stb::RectTextureData::loadFromFile(device, backPath));
    tex->faces.push_back(stb::RectTextureData::loadFromFile(device, leftPath));
    tex->faces.push_back(stb::RectTextureData::loadFromFile(device, rightPath));
    tex->faces.push_back(stb::RectTextureData::loadFromFile(device, topPath));
    tex->faces.push_back(stb::RectTextureData::loadFromFile(device, bottomPath));
    return tex;
}

auto stb::CubeTextureData::getMipLevelCount() const -> uint32_t
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

auto stb::CubeTextureData::getWidth(uint32_t mipLevel) const -> uint32_t
{
    return faces[0]->getWidth(mipLevel);
}

auto stb::CubeTextureData::getWidth(uint32_t face, uint32_t mipLevel) const -> uint32_t
{
    return faces[face]->getWidth(mipLevel);
}

auto stb::CubeTextureData::getHeight(uint32_t mipLevel) const -> uint32_t
{
    return faces[0]->getHeight(mipLevel);
}

auto stb::CubeTextureData::getHeight(uint32_t face, uint32_t mipLevel) const -> uint32_t
{
    return faces[face]->getHeight(mipLevel);
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
