/* 
 * Copyright (c) Aleksey Fedotov 
 * MIT license 
 */

#include "SoloTexture.h"
#include "SoloTexture.h"
#include "SoloLuaCommon.h"

using namespace solo;

static void registerTexture(CppBindModule<LuaBinding> &module)
{
    auto binding = BEGIN_CLASS(module, Texture);
    REG_METHOD(binding, Texture, format);
    REG_METHOD(binding, Texture, horizontalWrap);
    REG_METHOD(binding, Texture, verticalWrap);
    REG_METHOD(binding, Texture, setWrap);
    REG_METHOD(binding, Texture, setHorizontalWrap);
    REG_METHOD(binding, Texture, setVerticalWrap);
    REG_METHOD(binding, Texture, minFilter);
    REG_METHOD(binding, Texture, magFilter);
    REG_METHOD(binding, Texture, mipFilter);
    REG_METHOD(binding, Texture, setFilter);
    REG_METHOD(binding, Texture, anisotropyLevel);
    REG_METHOD(binding, Texture, setAnisotropyLevel);
    REG_PTR_EQUALITY(binding, Texture); // TODO not sure has any effect, it's not polymorphic anyway
    binding.endClass();
}

static void registerTexture2D(CppBindModule<LuaBinding> &module)
{
    {
        auto binding = BEGIN_CLASS_EXTEND(module, Texture2D, Texture);
        REG_STATIC_METHOD(binding, Texture2D, fromFile);
        REG_STATIC_METHOD(binding, Texture2D, fromFileAsync);
        REG_STATIC_METHOD(binding, Texture2D, empty);
        REG_METHOD(binding, Texture2D, dimensions);
        REG_PTR_EQUALITY(binding, Texture2D);
        binding.endClass();
    }
    {
        auto binding = BEGIN_CLASS_RENAMED(module, AsyncHandle<Texture2D>, "Texture2DAsyncHandle");
        REG_METHOD(binding, AsyncHandle<Texture2D>, done);
        binding.endClass();
    }
}

static void registerCubeTexture(CppBindModule<LuaBinding> &module)
{
    {
        auto binding = BEGIN_CLASS_EXTEND(module, CubeTexture, Texture);
        REG_STATIC_METHOD(binding, CubeTexture, fromFaceFiles);
        REG_STATIC_METHOD(binding, CubeTexture, fromFaceFilesAsync);
        REG_METHOD(binding, CubeTexture, depthWrap);
        REG_METHOD(binding, CubeTexture, setDepthWrap);
        REG_PTR_EQUALITY(binding, CubeTexture);
        binding.endClass();
    }
    {
        auto binding = BEGIN_CLASS_RENAMED(module, AsyncHandle<CubeTexture>, "CubeTextureAsyncHandle");
        REG_METHOD(binding, AsyncHandle<CubeTexture>, done);
        binding.endClass();
    }
}

void registerTextureApi(CppBindModule<LuaBinding> &module)
{
    registerTexture(module);
    registerTexture2D(module);
    registerCubeTexture(module);
}