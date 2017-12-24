/*
    Copyright (c) Aleksey Fedotov
    MIT license
*/

#include "SoloTexture.h"
#include "SoloTexture.h"
#include "SoloLuaCommon.h"

using namespace solo;

static void registerTexture(CppBindModule<LuaBinding> &module)
{
    auto tex = BEGIN_CLASS(module, Texture);
    REG_METHOD(tex, Texture, getHorizontalWrapping);
    REG_METHOD(tex, Texture, getVerticalWrapping);
    REG_METHOD(tex, Texture, setWrapping);
    REG_METHOD(tex, Texture, setHorizontalWrapping);
    REG_METHOD(tex, Texture, setVerticalWrapping);
    REG_METHOD(tex, Texture, getMinFiltering);
    REG_METHOD(tex, Texture, getMagFiltering);
    REG_METHOD(tex, Texture, setFiltering);
    REG_METHOD(tex, Texture, setMinFiltering);
    REG_METHOD(tex, Texture, setMagFiltering);
    REG_METHOD(tex, Texture, getAnisotropyLevel);
    REG_METHOD(tex, Texture, setAnisotropyLevel);
    tex.endClass();
}

static void registerTexture2d(CppBindModule<LuaBinding> &module)
{
    auto rectTex = BEGIN_CLASS_EXTEND(module, Texture2d, Texture);
    REG_STATIC_METHOD(rectTex, Texture2d, loadFromFile);
    REG_STATIC_METHOD(rectTex, Texture2d, loadFromFileAsync);
    REG_STATIC_METHOD(rectTex, Texture2d, createEmpty);
    REG_METHOD(rectTex, Texture2d, getDimensions);
    rectTex.endClass();
}

static void registerCubeTexture(CppBindModule<LuaBinding> &module)
{
    auto cubeTex = BEGIN_CLASS_EXTEND(module, CubeTexture, Texture);
    REG_STATIC_METHOD(cubeTex, CubeTexture, loadFromFaceFiles);
    REG_STATIC_METHOD(cubeTex, CubeTexture, loadFromFaceFilesAsync);
    REG_METHOD(cubeTex, CubeTexture, getDepthWrapping);
    REG_METHOD(cubeTex, CubeTexture, setDepthWrapping);
    cubeTex.endClass();
}

void registerTextureApi(CppBindModule<LuaBinding> &module)
{
    registerTexture(module);
    registerTexture2d(module);
    registerCubeTexture(module);
}