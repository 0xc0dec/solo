/*
    Copyright (c) Aleksey Fedotov
    MIT license
*/

#include "SoloTextureData.h"
#include "stb/SoloStbTextureData.h"

using namespace solo;

auto Texture2dData::loadFromFile(Device *device, const std::string &path) -> sptr<Texture2dData>
{
    if (stb::Texture2dData::canLoadFromFile(path))
        return stb::Texture2dData::loadFromFile(device, path);
    SL_PANIC("Unsupported cube texture file ", path);
    return nullptr;
}

auto CubeTextureData::loadFromFaceFiles(Device *device,
    const std::string &frontPath,
    const std::string &backPath,
    const std::string &leftPath,
    const std::string &rightPath,
    const std::string &topPath,
    const std::string &bottomPath) -> sptr<CubeTextureData>
{
    if (stb::CubeTextureData::canLoadFromFaceFiles(frontPath, backPath, leftPath, rightPath, topPath, bottomPath))
        return stb::CubeTextureData::loadFromFaceFiles(device, frontPath, backPath, leftPath, rightPath, topPath, bottomPath);
    SL_PANIC(SL_FMT("Unsupported cube texture face files ", frontPath, ", ..."));
    return nullptr;
}
