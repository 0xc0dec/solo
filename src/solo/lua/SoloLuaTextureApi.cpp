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
    REG_METHOD(tex, Texture, getFormat);
    REG_METHOD(tex, Texture, getHorizontalWrap);
    REG_METHOD(tex, Texture, getVerticalWrap);
    REG_METHOD(tex, Texture, setWrap);
    REG_METHOD(tex, Texture, setHorizontalWrap);
    REG_METHOD(tex, Texture, setVerticalWrap);
    REG_METHOD(tex, Texture, getMinFilter);
    REG_METHOD(tex, Texture, getMagFilter);
    REG_METHOD(tex, Texture, getMipFilter);
    REG_METHOD(tex, Texture, setFilter);
    REG_METHOD(tex, Texture, getAnisotropyLevel);
    REG_METHOD(tex, Texture, setAnisotropyLevel);
    tex.endClass();
}

static void registerTexture2D(CppBindModule<LuaBinding> &module)
{
    auto rectTex = BEGIN_CLASS_EXTEND(module, Texture2D, Texture);
    REG_STATIC_METHOD(rectTex, Texture2D, loadFromFile);
    REG_STATIC_METHOD(rectTex, Texture2D, loadFromFileAsync);
    REG_STATIC_METHOD(rectTex, Texture2D, createEmpty);
    REG_METHOD(rectTex, Texture2D, getDimensions);
    rectTex.endClass();
}

static void registerCubeTexture(CppBindModule<LuaBinding> &module)
{
    auto cubeTex = BEGIN_CLASS_EXTEND(module, CubeTexture, Texture);
    REG_STATIC_METHOD(cubeTex, CubeTexture, loadFromFaceFiles);
    REG_STATIC_METHOD(cubeTex, CubeTexture, loadFromFaceFilesAsync);
    REG_METHOD(cubeTex, CubeTexture, getDepthWrap);
    REG_METHOD(cubeTex, CubeTexture, setDepthWrap);
    cubeTex.endClass();
}

void registerTextureApi(CppBindModule<LuaBinding> &module)
{
    registerTexture(module);
    registerTexture2D(module);
    registerCubeTexture(module);
}