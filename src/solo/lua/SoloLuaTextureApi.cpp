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
    REG_METHOD(tex, Texture, format);
    REG_METHOD(tex, Texture, horizontalWrap);
    REG_METHOD(tex, Texture, verticalWrap);
    REG_METHOD(tex, Texture, setWrap);
    REG_METHOD(tex, Texture, setHorizontalWrap);
    REG_METHOD(tex, Texture, setVerticalWrap);
    REG_METHOD(tex, Texture, minFilter);
    REG_METHOD(tex, Texture, magFilter);
    REG_METHOD(tex, Texture, mipFilter);
    REG_METHOD(tex, Texture, setFilter);
    REG_METHOD(tex, Texture, anisotropyLevel);
    REG_METHOD(tex, Texture, setAnisotropyLevel);
    tex.endClass();
}

static void registerTexture2D(CppBindModule<LuaBinding> &module)
{
    auto rectTex = BEGIN_CLASS_EXTEND(module, Texture2D, Texture);
    REG_STATIC_METHOD(rectTex, Texture2D, fromFile);
    REG_STATIC_METHOD(rectTex, Texture2D, fromFileAsync);
    REG_STATIC_METHOD(rectTex, Texture2D, empty);
    REG_METHOD(rectTex, Texture2D, dimensions);
    rectTex.endClass();
}

static void registerCubeTexture(CppBindModule<LuaBinding> &module)
{
    auto cubeTex = BEGIN_CLASS_EXTEND(module, CubeTexture, Texture);
    REG_STATIC_METHOD(cubeTex, CubeTexture, fromFaceFiles);
    REG_STATIC_METHOD(cubeTex, CubeTexture, fromFaceFilesAsync);
    REG_METHOD(cubeTex, CubeTexture, depthWrap);
    REG_METHOD(cubeTex, CubeTexture, setDepthWrap);
    cubeTex.endClass();
}

void registerTextureApi(CppBindModule<LuaBinding> &module)
{
    registerTexture(module);
    registerTexture2D(module);
    registerCubeTexture(module);
}