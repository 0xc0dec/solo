/*
    Copyright (c) Aleksey Fedotov
    MIT license
*/

#include "SoloTexture.h"
#include "SoloRectTexture.h"
#include "SoloCubeTexture.h"
#include "SoloLuaCommon.h"

using namespace solo;

static void setRectTextureData(RectTexture *tex, ImageFormat format, const std::vector<uint8_t> &data, uint32_t width, uint32_t height)
{
    tex->setData(format, data.data(), width, height);
}

static void setCubeTextureData(CubeTexture *tex, CubeTextureFace face, ImageFormat format, const std::vector<uint8_t> &data, uint32_t width, uint32_t height)
{
    tex->setData(face, format, data.data(), width, height);
}

static void registerTexture(CppBindModule<LuaBinding> &module)
{
    auto tex = BEGIN_CLASS(module, Texture);
    REG_METHOD(tex, Texture, generateMipmaps);
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

static void registerRectTexture(CppBindModule<LuaBinding> &module)
{
    auto rectTex = BEGIN_CLASS_EXTEND(module, RectTexture, Texture);
    REG_STATIC_METHOD(rectTex, RectTexture, create);
    REG_FREE_FUNC_AS_METHOD_RENAMED(rectTex, setRectTextureData, "setData");
    REG_METHOD(rectTex, RectTexture, getSize);
    rectTex.endClass();
}

static void registerCubeTexture(CppBindModule<LuaBinding> &module)
{
    auto cubeTex = BEGIN_CLASS_EXTEND(module, CubeTexture, Texture);
    REG_STATIC_METHOD(cubeTex, CubeTexture, create);
    REG_FREE_FUNC_AS_METHOD_RENAMED(cubeTex, setCubeTextureData, "setData");
    REG_METHOD(cubeTex, CubeTexture, getDepthWrapping);
    REG_METHOD(cubeTex, CubeTexture, setDepthWrapping);
    cubeTex.endClass();
}

void registerTextureApi(CppBindModule<LuaBinding> &module)
{
    registerTexture(module);
    registerRectTexture(module);
    registerCubeTexture(module);
}